#include "exforwin/efw.h"

#define LIB_NAME                "efw"
#define LIB_VERSION             "1.3"

#include "baselib_ex.h"
#include "oslib_ex.h"
#include "loadlib_ex.h"
#include "iolib_ex.h"

LUAMOD_API int luaopen_efw(lua_State* L) {
    //�����������Ϊefw��ĳ�ʼ������ڣ���require 'efw'ʱ�ͻ����efw���а�����ģ��
    //��ν��ʼ�����ǵ��ò�ͬ��չģ��ļ��غ���
#ifdef WIN_UTF8
    luaload_base_ex(L);
    luaload_io_ex(L);
    luaload_os_ex(L);
    luaload_load_ex(L);
#endif
    return 0;
}