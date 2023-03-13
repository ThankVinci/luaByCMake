#pragma once

#ifdef LUA_BUILD_AS_DLL
#ifdef LUARNR_API
#define LUARNR_DLL_API __declspec(dllexport)
#endif // LUARNR_API
#else
#define LUARNR_DLL_API 
#endif

#include <stddef.h>
class LuaRnr_imp; //lua运行器的实际实现
class ITextFileReader; //抽象reader接口

class LUARNR_DLL_API  LuaRnr{

public:
	LuaRnr(const char* luaScript);
	LuaRnr(ITextFileReader* reader,const char* luafilePath);
	~LuaRnr();

public:
	void resetLuaRunner(); /*用于重置lua运行器，可在外部调用*/
	void pushArg(const char* arg); /*用于添加lua运行参数，可在外部调用*/
	bool ready(); /*用于判断Runner是否已经准备好运行，只有返回true时才可以调用run*/
	int run(); /*用于运行lua脚本，可在外部调用*/

public:
	size_t getArgListSize(); /*用于获取lua脚本执行的参数数量，可在外部调用*/
	size_t getRetvListSize(); /*用于获取lua脚本执行的返回值数量，可在外部调用*/
	const char* getRetvByIdx(size_t idx); /*用于获取某个下标的返回值，可在外部调用*/
	size_t getRetvSizeByIdx(size_t idx); /*用于获取某个下标的返回值的字节流数据大小，可在外部调用*/
	const char* getScript(); /*用于获取加载到Runner中的脚本，可在外部调用*/

public:
	void printScript(); /*打印出脚本内容*/
	void printArguments(); /*打印出参数列表*/
	void printCommand(); /*打印出完整命令*/
	void printAllRetvDataInfo(); /*打印出所有返回值的信息，包括数据大小和数据（以字符串的形式打印出来）*/
	void printAllRetvDataByteArray(); /*打印出所有返回值的信息，包括数据大小和数据（以字节数组的形式打印出来）*/

private:
	LuaRnr_imp* m_pImp;
};