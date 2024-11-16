#include "exforwin/efw.h"

#define LIB_NAME                "efw"
#define LIB_VERSION             "1.3"

#include "baselib_ex.h"
#include "oslib_ex.h"
#include "loadlib_ex.h"
#include "iolib_ex.h"

LUAMOD_API int luaopen_efw(lua_State* L) {
    //这个函数是作为efw库的初始化的入口，在require 'efw'时就会加载efw库中包含的模块
    //所谓初始化就是调用不同扩展模块的加载函数
#ifdef WIN_UTF8
    luaload_base_ex(L);
    luaload_io_ex(L);
    luaload_os_ex(L);
    luaload_load_ex(L);
#endif
    return 0;
}