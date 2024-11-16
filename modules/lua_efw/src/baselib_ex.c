#include "baselib_ex.h"

#ifdef WIN_UTF8
static int luaB_print_utf8(lua_State* L) {
    int n = lua_gettop(L);  /* number of arguments */
    int i;
    // 输出宽字符串前先把翻译模式改成_O_U16TEXT
    _setmode(_fileno(stdout), _O_U16TEXT);
    for (i = 1; i <= n; i++) {  /* for each argument */
        size_t l;
        const char* s_u8 = luaL_tolstring(L, i, &l);  /* convert it to string */
        WCHAR* s = U8StrtoU16Str(s_u8); /* 将char*的u8字符串转换为wchar*的u16字符串 */
        if (i > 1)  /* not the first element? */
            lua_writewstring(L"\t", 1);  /* add a tab before it */
        lua_writewstring(s, wcslen(s));
        free(s);
        lua_pop(L, 1);  /* pop result */
    }
    lua_writewline();
    // 输出完成并刷新后把翻译模式改回_O_TEXT
    _setmode(_fileno(stdout), _O_TEXT);
    return 0;
}

static const luaL_Reg base_funcs[] = {
  {"print", luaB_print_utf8}, //添加print输出utf8字符串支持
  {NULL, NULL}
};

int luaload_base_ex(lua_State* L) {
    lua_getglobal(L, LUA_GNAME); //对于base模块的函数，只需要设到_G表中即可
    luaL_setfuncs(L, base_funcs, 0);
    return 0;
}

#endif