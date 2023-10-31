#include "auxlib_ex.h"

// 注：本函数与lauxlib中的同名函数存在符号冲突，因此本函数并不可直接加入到源代码中
int luaL_loadfilex(lua_State* L, const char* filename,
    const char* mode) {
    LoadF lf;
    int status, readstatus;
    int c;
    int fnameindex = lua_gettop(L) + 1;  /* index of filename on the stack */
    wchar_t* w_filename;
    if (filename == NULL) {
        lua_pushliteral(L, "=stdin");
        lf.f = stdin;
    }
    else {
        lua_pushfstring(L, "@%s", filename);
        w_filename = U8StrtoU16Str(filename);
        lf.f = _wfopen(w_filename, L"r");
        if (lf.f == NULL) return errfile(L, "open", fnameindex);
    }
    lf.n = 0;
    if (skipcomment(lf.f, &c))  /* read initial portion */
        lf.buff[lf.n++] = '\n';  /* add newline to correct line numbers */
    if (c == LUA_SIGNATURE[0]) {  /* binary file? */
        lf.n = 0;  /* remove possible newline */
        if (w_filename) {  /* "real" file? */
            lf.f = _wfreopen(w_filename, L"rb", lf.f);  /* reopen in binary mode */
            if (lf.f == NULL) return errfile(L, "reopen", fnameindex);
            skipcomment(lf.f, &c);  /* re-read initial portion */
        }
    }
    free(w_filename);
    if (c != EOF)
        lf.buff[lf.n++] = c;  /* 'c' is the first character of the stream */
    status = lua_load(L, getF, &lf, lua_tostring(L, -1), mode);
    readstatus = ferror(lf.f);
    if (filename) fclose(lf.f);  /* close file (even in case of errors) */
    if (readstatus) {
        lua_settop(L, fnameindex);  /* ignore results from 'lua_load' */
        return errfile(L, "read", fnameindex);
    }
    lua_remove(L, fnameindex);
    return status;
}


//static const luaL_Reg aux_funcs[] = {
//  {"dofile", luaB_dofile_utf8},
//  {"print", luaB_print_utf8},
//  {NULL, NULL}
//};