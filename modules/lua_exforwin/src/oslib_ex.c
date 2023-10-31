#include "oslib_ex.h"

#define l_wsystem(cmd)	_wsystem(cmd)  /* default definition */

static int os_execute(lua_State *L) {
  //_setmode(_fileno(stdout), _O_U16TEXT);
  const char *cmd = luaL_optstring(L, 1, NULL); //获取UTF-8字符串
  size_t cmdLength = strlen(cmd)+1;
  wchar_t* cmd_u16 = U8StrtoU16Str(cmd);
  int stat;
  errno = 0;
  stat = l_wsystem(cmd_u16);
  if (cmd != NULL)
    return luaL_execresult(L, stat);
  else {
    lua_pushboolean(L, stat);  /* true if there is a shell */
    return 1;
  }
}

static const struct luaL_Reg efwlib[] = {
	{"execute",   os_execute},
	{NULL,NULL}
};

LUAMOD_API int luaopen_os_ex(lua_State *L) {
  luaL_newlib(L, efwlib);
  return 1;
}