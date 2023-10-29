#include "efw.h"

#define LIB_NAME                "efw"
#define LIB_VERSION             "1.0"

static const luaL_Reg loadedlibs[] = {
  {LUA_EFW_BASE, luaopen_base_ex},
  {LUA_EFW_OS, luaopen_os_ex},
  {NULL, NULL}
};

LUALIB_API int luaopen_efw(lua_State* L) {
    const luaL_Reg* lib;
    /* "require" functions from 'loadedlibs' and set results to global table */
    for (lib = loadedlibs; lib->func; lib++) {
        luaL_requiref(L, lib->name, lib->func, 1);
        lua_pop(L, 1);  /* remove lib */
    }
    return 0;
}