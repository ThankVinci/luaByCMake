﻿/*
** $Id: lua.c $
** Lua stand-alone interpreter
** See Copyright Notice in lua.h
*/

#define lua_c

#include "lprefix.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"


#if !defined(LUA_PROGNAME)
#define LUA_PROGNAME		"lua"
#endif

#if !defined(LUA_INIT_VAR)
#define LUA_INIT_VAR		"LUA_INIT"
#endif

#define LUA_INITVARVERSION	LUA_INIT_VAR LUA_VERSUFFIX


static lua_State* globalL = NULL;

static const char* progname = LUA_PROGNAME;


#if defined(LUA_USE_POSIX)   /* { */

/*
** Use 'sigaction' when available.
*/
static void setsignal(int sig, void (*handler)(int)) {
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);  /* do not mask any signal */
    sigaction(sig, &sa, NULL);
}

#else           /* }{ */

#define setsignal            signal

#endif                               /* } */


/*
** Hook set by signal function to stop the interpreter.
*/
static void lstop(lua_State* L, lua_Debug* ar) {
    (void)ar;  /* unused arg. */
    lua_sethook(L, NULL, 0, 0);  /* reset hook */
    luaL_error(L, "interrupted!");
}


/*
** Function to be called at a C signal. Because a C signal cannot
** just change a Lua state (as there is no proper synchronization),
** this function only sets a hook that, when called, will stop the
** interpreter.
*/
static void laction(int i) {
    int flag = LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE | LUA_MASKCOUNT;
    setsignal(i, SIG_DFL); /* if another SIGINT happens, terminate process */
    lua_sethook(globalL, lstop, flag, 1);
}


static void print_usage(const char* badoption) {
    lua_writestringerror("%s: ", progname);
    if (badoption[1] == 'e' || badoption[1] == 'l')
        lua_writestringerror("'%s' needs argument\n", badoption);
    else
        lua_writestringerror("unrecognized option '%s'\n", badoption);
    lua_writestringerror(
        "usage: %s [options] [script [args]]\n"
        "Available options are:\n"
        "  -e stat   execute string 'stat'\n"
        "  -i        enter interactive mode after executing 'script'\n"
        "  -l mod    require library 'mod' into global 'mod'\n"
        "  -l g=mod  require library 'mod' into global 'g'\n"
        "  -v        show version information\n"
        "  -E        ignore environment variables\n"
        "  -W        turn warnings on\n"
        "  --        stop handling options\n"
        "  -         stop handling options and execute stdin\n"
        ,
        progname);
}


/*
** Prints an error message, adding the program name in front of it
** (if present)
*/
static void l_message(const char* pname, const char* msg) {
    if (pname) lua_writestringerror("%s: ", pname);
    lua_writestringerror("%s\n", msg);
}


/*
** Check whether 'status' is not OK and, if so, prints the error
** message on the top of the stack. It assumes that the error object
** is a string, as it was either generated by Lua or by 'msghandler'.
*/
static int report(lua_State* L, int status) {
    if (status != LUA_OK) {
        const char* msg = lua_tostring(L, -1);
        l_message(progname, msg);
        lua_pop(L, 1);  /* remove message */
    }
    return status;
}


/*
** Message handler used to run all chunks
*/
static int msghandler(lua_State* L) {
    const char* msg = lua_tostring(L, 1);
    if (msg == NULL) {  /* is error object not a string? */
        if (luaL_callmeta(L, 1, "__tostring") &&  /* does it have a metamethod */
            lua_type(L, -1) == LUA_TSTRING)  /* that produces a string? */
            return 1;  /* that is the message */
        else
            msg = lua_pushfstring(L, "(error object is a %s value)",
                luaL_typename(L, 1));
    }
    luaL_traceback(L, L, msg, 1);  /* append a standard traceback */
    return 1;  /* return the traceback */
}


/*
** Interface to 'lua_pcall', which sets appropriate message function
** and C-signal handler. Used to run all chunks.
*/
static int docall(lua_State* L, int narg, int nres) {
    int status;
    int base = lua_gettop(L) - narg;  /* function index */
    lua_pushcfunction(L, msghandler);  /* push message handler */
    lua_insert(L, base);  /* put it under function and args */
    globalL = L;  /* to be available to 'laction' */
    setsignal(SIGINT, laction);  /* set C-signal handler */
    status = lua_pcall(L, narg, nres, base);
    setsignal(SIGINT, SIG_DFL); /* reset C-signal handler */
    lua_remove(L, base);  /* remove message handler from the stack */
    return status;
}


static void print_version(void) {
    lua_writestring(LUA_COPYRIGHT, strlen(LUA_COPYRIGHT));
    lua_writeline();
}


/*
** Create the 'arg' table, which stores all arguments from the
** command line ('argv'). It should be aligned so that, at index 0,
** it has 'argv[script]', which is the script name. The arguments
** to the script (everything after 'script') go to positive indices;
** other arguments (before the script name) go to negative indices.
** If there is no script name, assume interpreter's name as base.
*/
static void createargtable(lua_State* L, char** argv, int argc, int script) {
    int i, narg;
    if (script == argc) script = 0;  /* no script name? */
    narg = argc - (script + 1);  /* number of positive indices */
    lua_createtable(L, narg, script + 1);
    for (i = 0; i < argc; i++) {
        lua_pushstring(L, argv[i]);
        lua_rawseti(L, -2, i - script);
    }
    lua_setglobal(L, "arg");
}


static int dochunk(lua_State* L, int status) {
    if (status == LUA_OK) status = docall(L, 0, 0);
    return report(L, status);
}


static int dofile(lua_State* L, const char* name) {
    return dochunk(L, luaL_loadfile(L, name));
}


static int dostring(lua_State* L, const char* s, const char* name) {
    return dochunk(L, luaL_loadbuffer(L, s, strlen(s), name));
}


/*
** Receives 'globname[=modname]' and runs 'globname = require(modname)'.
*/
static int dolibrary(lua_State* L, char* globname) {
    int status;
    char* modname = strchr(globname, '=');
    if (modname == NULL)  /* no explicit name? */
        modname = globname;  /* module name is equal to global name */
    else {
        *modname = '\0';  /* global name ends here */
        modname++;  /* module name starts after the '=' */
    }
    lua_getglobal(L, "require");
    lua_pushstring(L, modname);
    status = docall(L, 1, 1);  /* call 'require(modname)' */
    if (status == LUA_OK)
        lua_setglobal(L, globname);  /* globname = require(modname) */
    return report(L, status);
}


/*
** Push on the stack the contents of table 'arg' from 1 to #arg
*/
static int pushargs(lua_State* L) {
    int i, n;
    if (lua_getglobal(L, "arg") != LUA_TTABLE)
        luaL_error(L, "'arg' is not a table");
    n = (int)luaL_len(L, -1);
    luaL_checkstack(L, n + 3, "too many arguments to script");
    for (i = 1; i <= n; i++)
        lua_rawgeti(L, -i, i);
    lua_remove(L, -i);  /* remove table from the stack */
    return n;
}


static int handle_script(lua_State* L, char** argv) {
    int status;
    const char* fname = argv[0];
    if (strcmp(fname, "-") == 0 && strcmp(argv[-1], "--") != 0)
        fname = NULL;  /* stdin */
    status = luaL_loadfile(L, fname);
    if (status == LUA_OK) {
        int n = pushargs(L);  /* push arguments to script */
        status = docall(L, n, LUA_MULTRET);
    }
    return report(L, status);
}


/* bits of various argument indicators in 'args' */
#define has_error	1	/* bad option */
#define has_i		2	/* -i */
#define has_v		4	/* -v */
#define has_e		8	/* -e */
#define has_E		16	/* -E */


/*
** Traverses all arguments from 'argv', returning a mask with those
** needed before running any Lua code (or an error code if it finds
** any invalid argument). 'first' returns the first not-handled argument
** (either the script name or a bad argument in case of error).
*/
static int collectargs(char** argv, int* first) {
    int args = 0;
    int i;
    for (i = 1; argv[i] != NULL; i++) {
        *first = i;
        if (argv[i][0] != '-')  /* not an option? */
            return args;  /* stop handling options */
        switch (argv[i][1]) {  /* else check option */
        case '-':  /* '--' */
            if (argv[i][2] != '\0')  /* extra characters after '--'? */
                return has_error;  /* invalid option */
            *first = i + 1;
            return args;
        case '\0':  /* '-' */
            return args;  /* script "name" is '-' */
        case 'E':
            if (argv[i][2] != '\0')  /* extra characters? */
                return has_error;  /* invalid option */
            args |= has_E;
            break;
        case 'W':
            if (argv[i][2] != '\0')  /* extra characters? */
                return has_error;  /* invalid option */
            break;
        case 'i':
            args |= has_i;  /* (-i implies -v) *//* FALLTHROUGH */
        case 'v':
            if (argv[i][2] != '\0')  /* extra characters? */
                return has_error;  /* invalid option */
            args |= has_v;
            break;
        case 'e':
            args |= has_e;  /* FALLTHROUGH */
        case 'l':  /* both options need an argument */
            if (argv[i][2] == '\0') {  /* no concatenated argument? */
                i++;  /* try next 'argv' */
                if (argv[i] == NULL || argv[i][0] == '-')
                    return has_error;  /* no next argument or it is another option */
            }
            break;
        default:  /* invalid option */
            return has_error;
        }
    }
    *first = i;  /* no script name */
    return args;
}


/*
** Processes options 'e' and 'l', which involve running Lua code, and
** 'W', which also affects the state.
** Returns 0 if some code raises an error.
*/
static int runargs(lua_State* L, char** argv, int n) {
    int i;
    for (i = 1; i < n; i++) {
        int option = argv[i][1];
        lua_assert(argv[i][0] == '-');  /* already checked */
        switch (option) {
        case 'e':  case 'l': {
            int status;
            char* extra = argv[i] + 2;  /* both options need an argument */
            if (*extra == '\0') extra = argv[++i];
            lua_assert(extra != NULL);
            status = (option == 'e')
                ? dostring(L, extra, "=(command line)")
                : dolibrary(L, extra);
            if (status != LUA_OK) return 0;
            break;
        }
        case 'W':
            lua_warning(L, "@on", 0);  /* warnings on */
            break;
        }
    }
    return 1;
}


static int handle_luainit(lua_State* L) {
    const char* name = "=" LUA_INITVARVERSION;
    const char* init = getenv(name + 1);
    if (init == NULL) {
        name = "=" LUA_INIT_VAR;
        init = getenv(name + 1);  /* try alternative name */
    }
    if (init == NULL) return LUA_OK;
    else if (init[0] == '@')
        return dofile(L, init + 1);
    else
        return dostring(L, init, name);
}


/*
** {==================================================================
** Read-Eval-Print Loop (REPL)
** ===================================================================
*/

#if !defined(LUA_PROMPT)
#define LUA_PROMPT		"> "
#define LUA_PROMPT2		">> "
#endif

#if !defined(LUA_MAXINPUT)
#define LUA_MAXINPUT		512
#endif


/*
** lua_stdin_is_tty detects whether the standard input is a 'tty' (that
** is, whether we're running lua interactively).
*/
#if !defined(lua_stdin_is_tty)	/* { */

#if defined(LUA_USE_POSIX)	/* { */

#include <unistd.h>
#define lua_stdin_is_tty()	isatty(0)

#elif defined(LUA_USE_WINDOWS)	/* }{ */

#include <io.h>
#include <windows.h>

#define lua_stdin_is_tty()	_isatty(_fileno(stdin))

#else				/* }{ */

/* ISO C definition */
#define lua_stdin_is_tty()	1  /* assume stdin is a tty */

#endif				/* } */

#endif				/* } */

/* }================================================================== */

static int runLuaScript(lua_State* L) {
    //获取入参
    int argc = (int)lua_tointeger(L, 1);
    char** argv = (char**)lua_touserdata(L, 2);
    int script;
    int args = collectargs(argv, &script);

    //检查和初始化的操作
    luaL_checkversion(L); //检查版本
    luaL_openlibs(L); //加载lua自带的标准库
    createargtable(L, argv, argc, script);
    lua_gc(L, LUA_GCGEN, 0, 0); //控制垃圾收集器，不知道这和GCGEN的作用是什么，反正默认抄过来就是了
    if (handle_luainit(L) != LUA_OK)  return 0; //进行lua初始化，初始化失败就退出

    //初始化结束后就是自定义的操作了
    const char* name = argv[0];
    const char* luaScript = argv[1];

    //加载字符串作为lua脚本然后进行调用
    luaL_loadstring(L, luaScript); 
    docall(L, 0, 1); //docall第三个参数告知运行脚本之后会得到一个返回值，这个返回值会入栈
    lua_pushboolean(L, 1);  //返回值true入栈
    return 2;
}
#include "rlua.h"

int runLua(int argc, const char** argv, int* retc, char*** retv, unsigned long long** rets) {
    int status, result, idx;
    lua_State* L = luaL_newstate();  // 创建空栈
    if (L == NULL) {
        l_message(argv[0], "cannot create state: not enough memory");
        return EXIT_FAILURE;
    }
    //函数、参数入栈
    lua_pushcfunction(L, &runLuaScript);  //运行脚本的入口函数压入栈
    lua_pushinteger(L, argc); // 往栈中压入第一个参数，表示要执行脚本传入的参数数量
    lua_pushlightuserdata(L, argv); // 往栈中压入第二个参数，表示要执行脚本传入的参数列表
    //此时栈底为函数，往上依次是参数数量和参数列表
    //调用函数并返回结果
    status = docall(L, 2, 2);
    //docall第二个参数表示调用函数传入的参数数量，传入2就表示，从栈顶往下第三个元素表示函数，
    //然后传入栈顶往下两个元素作为参数，第三个参数就表示虚拟栈中的返回值有2个
    //执行之后栈顶为true、栈顶第二个元素为脚本的返回值
    result = lua_toboolean(L, -1);  // 获取栈顶的元素，用于检测脚本执行是否成功
    lua_pop(L, 1); //把虚拟栈顶的元素弹出，此时栈顶就是脚本的返回值

    //这里我假设我在脚本中返回的是table类型，如果不是table的话，说明这个脚本我不需要返回值，只需要执行就可以
    int is_table = lua_istable(L, -1);
    if (!is_table || retc == NULL || retv == NULL || rets == NULL) return (result && status == LUA_OK) ? EXIT_SUCCESS : EXIT_FAILURE;

    //运行到这里，虚拟栈中只有一个元素，是脚本的返回值
    //以下代码用于处理从脚本中的返回值

    /*获取key为retc的数值 表示返回值的数量 */
    lua_pushstring(L, "retc"); //此时栈顶变成了压入的字符串key
    lua_rawget(L, -2); //执行完这一行，原先栈顶代表key的字符串弹出，从表中获取到的value压到栈顶（整数数据）
    int toptype = lua_type(L, -1); 
    if (toptype == LUA_TNIL) {
        //获得栈顶的元素类型，为0就是nil，也就是说，当rawget获取不到key的value的时候，栈顶就会压入一个nil，但是不会影响到正常的执行，因为如果是nil的话，retc就会得到0，下面的操作自然就无效了。
    }
    *retc = (int)lua_tointeger(L, -1);
    lua_pop(L, 1); //弹出栈顶，此时栈顶变回承载返回值的table
    /***************************************/
    //当retc为0时，这里的操作就无效了
    /*获取key为rets的数值 表示返回值的元素大小 */
    lua_pushstring(L, "rets"); //此时栈顶变成了我压入的字符串key
    lua_rawget(L, -2); //执行完这一行，原先栈顶代表key的字符串弹出，从表中获取到的value压到栈顶（表数据）
    toptype = lua_type(L, -1);
    if (toptype == LUA_TNIL) {
        //当rawget获取不到key的value的时候，栈顶就会压入一个nil，这时栈顶肯定不是一个table，所以设置retc为0，这样下面的操作就无效了
        *retc = 0;
        *rets = NULL;
    }
    else {
        *rets = (unsigned long long*)malloc(sizeof(unsigned long long) * (*retc));
    }
    for (idx = 0; idx < *retc; idx++) {
        lua_rawgeti(L, -1, (lua_Integer)idx + 1); //从子表中获取数据压到栈顶
        (*rets)[idx] = lua_tounsigned(L, -1); //获取栈顶数据
        lua_pop(L, 1); //弹出栈顶，此时栈顶变回返回值中的子表
    }
    lua_pop(L, 1); //再次弹出栈顶的子表(或者是nil)，此时栈顶变回承载返回值的table
    /*******************************************/
    //当retc为0时，这里的操作就无效了
    /*获取key为retv的数值 表示返回值的列表 */
    lua_pushstring(L, "retv"); //此时栈顶变成了我压入的字符串key
    lua_rawget(L, -2); //执行完这一行，原先栈顶代表key的字符串弹出，从表中获取到的value压到栈顶（表数据）
    toptype = lua_type(L, -1);
    if (toptype == LUA_TNIL) {
        //当rawget获取不到key的value的时候，栈顶就会压入一个nil，这时栈顶肯定不是一个table，所以设置retc为0，这样下面的操作就无效了
        *retc = 0;
        *retv = NULL;
    }
    else {
        *retv = (char**)malloc(sizeof(char*) * (*retc));
    }
    for (idx = 0; idx < *retc; idx++) {
        lua_rawgeti(L, -1, (lua_Integer)idx + 1); //从子表中获取数据压到栈顶
        //由于不确定字符串的指针什么时候会被回收，在MSVC_DEBUG下确定内存是会被回收的，所以先进行拷贝
        char* buff = (char*)malloc(sizeof(char) * ((*rets)[idx]));
        char* src = lua_tostring(L, -1);
        memcpy(buff,src, (*rets)[idx]);
        (*retv)[idx] = buff; //获取栈顶数据
        lua_pop(L, 1); //弹出栈顶，此时栈顶变回返回值中的子表
    }
    lua_pop(L, 1); //再次弹出栈顶的子表，此时栈顶变回承载返回值的table
    /***************************************/

    lua_pop(L, 1); //把栈顶的脚本返回值table弹出，此时栈中没有元素了
    lua_close(L); //关闭虚拟栈
    return (result && status == LUA_OK) ? EXIT_SUCCESS : EXIT_FAILURE;
}

#ifdef RLUA_EXEC_MAIN

//读取整个文件
static char* readFileAsString(const char* luafilePath) {
    FILE* fpLuaFile = fopen(luafilePath, "r");
    if (fpLuaFile == NULL) return NULL;
    fseek(fpLuaFile, 0, SEEK_END);
    size_t luafileSize = ftell(fpLuaFile);
    fseek(fpLuaFile, 0, SEEK_SET);
    char* luafileBuff = (char*)malloc(sizeof(char) * (luafileSize+1));
    if (luafileBuff == NULL) return NULL;
    luafileSize = fread(luafileBuff, 1, luafileSize, fpLuaFile);
    luafileBuff[luafileSize] = '\0';
    fclose(fpLuaFile);
    return luafileBuff;
}


//模拟demo
//不要无脑运行这个主函数，要注意传入的参数和个数是否对应，读取的lua文件要能够找得到
int main(int argc2,char** argv2) {
    int argc = 4;
    char** argv = (char**)malloc(sizeof(char*)*argc);
    if (argv == NULL) return EXIT_FAILURE;
    argv[0] = "runScript";
    argv[2] = "arg1";
    argv[3] = "arg2";
    
    //lua文件读取
    const char* luafilePath;
    if (argc2 > 1) luafilePath = argv2[1];
    else luafilePath = "C:\\Users\\JohnSmith\\Desktop\\main.lua";
    argv[1] = readFileAsString(luafilePath);
    if(argv[1] == NULL) return EXIT_FAILURE;

    int retc = 0;
    char** retv = NULL;
    int result = runLua(argc, argv,&retc,&retv);
    for (int i = 0; i < retc;i++) {
        printf("%s\n",retv[i]);
    }
    //运行脚本结束后释放之前动态分配的空间
    free(argv[1]);
    free(argv);

    return 0;
}

#endif