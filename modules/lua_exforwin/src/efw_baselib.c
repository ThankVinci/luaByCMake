#define LUA_EFW_BASELIB
#include "lprefix.h"

#include <ctype.h>
#include "efw.h"

static int luaB_print(lua_State* L) {
    int n = lua_gettop(L);  /* number of arguments */
    int i;
    for (i = 1; i <= n; i++) {  /* for each argument */
        size_t l;
        
#ifdef WIN_UTF8
        _setmode(_fileno(stdout), _O_U16TEXT);
        const char* s_u8 = luaL_tolstring(L, i, &l);  /* convert it to string */
        WCHAR* s = U8StrtoU16Str(s_u8);
#else
        const char* s = luaL_tolstring(L, i, &l);  /* convert it to string */
#endif
        if (i > 1)  /* not the first element? */
            lua_writestring("\t", 1);  /* add a tab before it */
#ifdef WIN_UTF8
        wprintf(L"%ls\n",s);
        free(s);
#else
        lua_writestring(s, l);  /* print it */
#endif
        lua_pop(L, 1);  /* pop result */
    }
    lua_writeline();
    return 0;
}

static const luaL_Reg base_funcs[] = {
  {"print", luaB_print},
  {NULL, NULL}
};


LUALIB_API int luaopen_base_ex(lua_State* L) {
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