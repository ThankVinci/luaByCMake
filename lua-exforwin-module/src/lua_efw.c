#include "lua_efw.h"

#define LIB_NAME                "efw"
#define LIB_VERSION             "1.0"

#if defined(LUA_BUILD_AS_DLL) /**LUA_BUILD_AS_DLL只有在Windows构建lua时才有这个宏，所以在这个ifdef下面include<wchar.h>
并且以下的模块API定义均在这个宏下面，那么其他平台编译这个模块也没有意义，根本没有符号
*/
#include <wchar.h>
#include <io.h>
#include <fcntl.h>

#if !defined(l_wsystem) //模仿
#if defined(LUA_USE_IOS)
/* Despite claiming to be ISO C, iOS does not implement 'system'. */
#define l_wsystem(cmd) ((cmd) == NULL ? 0 : -1)
#else
#define l_wsystem(cmd)	_wsystem(cmd)  /* default definition */
#endif
#endif

static int os_execute(lua_State *L) {
  //_setmode(_fileno(stdout), _O_U16TEXT);
  const char *cmd = luaL_optstring(L, 1, NULL);
  int stat;
  errno = 0;
  stat = l_wsystem((const wchar_t*)cmd);
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

LUALIB_API int luaopen_efw (lua_State *L) {
  luaL_newlib(L, efwlib);
  return 1;
}

#endif //LUA_BUILD_AS_DLL