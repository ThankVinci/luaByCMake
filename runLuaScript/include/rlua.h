#ifndef _WIN32 //如果没有_WIN32的宏，说明不是MSVC项目，自然就不用导出接口
#define RLUA_DLL_API
#endif

#ifndef RLUA_DLL_API
#define RLUA_DLL_API __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*
* 最后一个参数如果是unsigned int的话，能够表示的字节流数据大小最大为32位，也就是4GB
* 如果是size_t的话，unsigned long long能够表示的字节流数据大小最大为64位，也就是16EB
* 但size_t包装成库后可能有问题，故用unsigned long long
*/
RLUA_DLL_API int runLua(int argc, const char** argv, int* retc, char*** retv, unsigned long long** rets);

#ifdef __cplusplus
}
#endif