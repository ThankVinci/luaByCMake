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

static int os_remove_utf8(lua_State* L) {
    const char* filename = luaL_checkstring(L, 1);
    wchar_t* w_filename = U8StrtoU16Str(filename);
    errno = 0;
    int result = _wremove(w_filename);
    free(w_filename);
    return luaL_fileresult(L, result == 0, filename);
}


static int os_rename_utf8(lua_State* L) {
    const char* fromname = luaL_checkstring(L, 1);
    const char* toname = luaL_checkstring(L, 2);
    wchar_t* w_fromname = U8StrtoU16Str(fromname);
    wchar_t* w_toname = U8StrtoU16Str(toname);
    errno = 0;
    int result = _wrename(w_fromname, w_toname);
    free(w_fromname);
    free(w_toname);
    return luaL_fileresult(L, result == 0, NULL);
}

static const struct luaL_Reg oslib_funcs[] = {
	{"execute",   os_execute_utf8},
    {"remove",    os_remove_utf8},
    {"rename",    os_rename_utf8},
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