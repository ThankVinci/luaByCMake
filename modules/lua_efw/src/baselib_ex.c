#include "baselib_ex.h"

#ifdef WIN_UTF8
static int luaB_print_utf8(lua_State* L) {
    int n = lua_gettop(L);  /* number of arguments */
    int i;
    // ������ַ���ǰ�Ȱѷ���ģʽ�ĳ�_O_U16TEXT
    _setmode(_fileno(stdout), _O_U16TEXT);
    for (i = 1; i <= n; i++) {  /* for each argument */
        size_t l;
        const char* s_u8 = luaL_tolstring(L, i, &l);  /* convert it to string */
        WCHAR* s = U8StrtoU16Str(s_u8); /* ��char*��u8�ַ���ת��Ϊwchar*��u16�ַ��� */
        if (i > 1)  /* not the first element? */
            lua_writewstring(L"\t", 1);  /* add a tab before it */
        lua_writewstring(s, wcslen(s));
        free(s);
        lua_pop(L, 1);  /* pop result */
    }
    lua_writewline();
    // �����ɲ�ˢ�º�ѷ���ģʽ�Ļ�_O_TEXT
    _setmode(_fileno(stdout), _O_TEXT);
    return 0;
}

static const luaL_Reg base_funcs[] = {
  {"print", luaB_print_utf8}, //���print���utf8�ַ���֧��
  {NULL, NULL}
};

int luaload_base_ex(lua_State* L) {
    lua_getglobal(L, LUA_GNAME); //����baseģ��ĺ�����ֻ��Ҫ�赽_G���м���
    luaL_setfuncs(L, base_funcs, 0);
    return 0;
}

#endif