#pragma once

#ifdef LUA_BUILD_AS_DLL
#ifdef LUARNR_API
#define LUARNR_DLL_API __declspec(dllexport)
#else
#define LUARNR_DLL_API 
#endif // LUARNR_API

#endif

#ifndef LUARNR_DLL_API

#endif

class LuaRnr_imp; //lua运行器的实际实现
class ITextFileReader; //抽象reader接口

class LUARNR_DLL_API LuaRnr{
private:
	LuaRnr_imp* m_pImp;
public:
	LuaRnr(const char* luaScript);
	LuaRnr(ITextFileReader* reader,const char* luafilePath);
		
	void addArg(const char* arg);

	int run();

    char* getIdx(size_t idx);

    size_t getRetvSize();

	void printRetv();

	~LuaRnr();
};