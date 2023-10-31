#include "efw.h"

#define LIB_NAME                "efw"
#define LIB_VERSION             "1.2"

#include "baselib_ex.h"
#include "oslib_ex.h"

#ifdef WIN_UTF8

static const luaL_Reg loadedlibs[] = {
  {BASE_LIB_EX, luaopen_base_ex},
  {OS_LIB_EX, luaopen_os_ex},
  {NULL, NULL}
};

LUAMOD_API int luaopen_efw(lua_State* L) { //�����������Ϊefw��ĳ�ʼ������ڣ���require 'efw'ʱ�ͻ����efw���а�����ģ��
    const luaL_Reg* lib;
    /* "require" functions from 'loadedlibs' and set results to global table */
    for (lib = loadedlibs; lib->func; lib++) {
        luaL_requiref(L, lib->name, lib->func, 1);
        lua_pop(L, 1);  /* remove lib */
    }
    return 0;
}

#else
LUALIB_API int luaopen_efw(lua_State* L) { //����ϵͳ�½��б����õ�һ���յĽ��
    return 0;
}
#endif