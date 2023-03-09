#ifndef _WIN32 //如果没有_WIN32的宏，说明不是MSVC项目，自然就不用导出接口
#define RLUA_DLL_API
#endif

#ifndef RLUA_DLL_API
#define RLUA_DLL_API __declspec(dllexport)
#endif

RLUA_DLL_API int runLua(int argc, char** argv, int* retc, char*** retv);