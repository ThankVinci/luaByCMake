#include "oslib_ex.h"

static int os_execute_utf8(lua_State *L) {
  const char *cmd = luaL_optstring(L, 1, NULL); //获取UTF-8字符串
  wchar_t* w_cmd = U8StrtoU16Str(cmd);
  int stat;
  errno = 0;
  stat = _wsystem(w_cmd);
  free(w_cmd);
  if (cmd != NULL)
    return luaL_execresult(L, stat);
  else {
    lua_pushboolean(L, stat);  /* true if there is a shell */
    return 1;
  }
}

static const struct luaL_Reg oslib_funcs[] = {
	{"execute",   os_execute_utf8},
	{NULL,NULL}
};

int luaload_os_ex(lua_State *L) {
    lua_getglobal(L, LUA_GNAME);
    lua_getfield(L,-1, LUA_OSLIBNAME); //获取全局的os表移到栈顶
    if (lua_istable(L,-1)) {
        luaL_setfuncs(L, oslib_funcs, 0);
    }
    else {
        //如果栈顶不是一个表（不太可能哈）,就插入一个新表
        lua_newtable(L);
        luaL_setfuncs(L, oslib_funcs, 0);
        lua_pushstring(L, LUA_OSLIBNAME);
        lua_pushvalue(L, -2);
        lua_setglobal(L, LUA_OSLIBNAME);
    }
    return 0;
}