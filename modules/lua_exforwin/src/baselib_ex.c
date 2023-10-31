#include "baselib_ex.h"
#include "auxlib_ex.h"

#ifdef WIN_UTF8

static int dofilecont(lua_State* L, int d1, lua_KContext d2) {
    (void)d1;  (void)d2;  /* only to match 'lua_Kfunction' prototype */
    return lua_gettop(L) - 1;
}

static int luaB_dofile_utf8(lua_State* L) {
    const char* fname = luaL_optstring(L, 1, NULL);
    lua_settop(L, 1);
    if (l_unlikely(luaL_loadfile(L, fname) != LUA_OK))
        return lua_error(L);
    lua_callk(L, 0, LUA_MULTRET, 0, dofilecont);
    return dofilecont(L, 0, 0);
}

static int luaB_print_utf8(lua_State* L) {
    int n = lua_gettop(L);  /* number of arguments */
    int i;
    for (i = 1; i <= n; i++) {  /* for each argument */
        size_t l;
        _setmode(_fileno(stdout), _O_U16TEXT);
        const char* s_u8 = luaL_tolstring(L, i, &l);  /* convert it to string */
        WCHAR* s = U8StrtoU16Str(s_u8);
        if (i > 1)  /* not the first element? */
            wprintf(L"\t");;  /* add a tab before it */
        wprintf(L"%ls",s);
        free(s);
        lua_pop(L, 1);  /* pop result */
    }
    wprintf(L"\n");
    return 0;
}

static const luaL_Reg base_funcs[] = {
  {"dofile", luaB_dofile_utf8}, //添加dofile能够执行utf8文件名的lua模块
  {"print", luaB_print_utf8}, //添加print输出utf8字符串支持
  {NULL, NULL}
};

int luaopen_base_ex(lua_State* L) {
    /* open lib into global table */
    lua_pushglobaltable(L);
    luaL_setfuncs(L, base_funcs, 0);
    /* set global _G */
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, LUA_GNAME);
    /* set global _VERSION */
    lua_pushliteral(L, LUA_VERSION);
    lua_setfield(L, -2, "_VERSION");
    return 1;
}

#endif