#pragma once

#ifdef LUA_BUILD_AS_DLL
#ifdef LUARNR_API
#define LUARNR_DLL_API __declspec(dllexport)
#endif // LUARNR_API
#else
#define LUARNR_DLL_API 
#endif

#include <stddef.h>
class LuaRnr_imp; //lua��������ʵ��ʵ��
class ITextFileReader; //����reader�ӿ�

class LUARNR_DLL_API  LuaRnr{

public:
	LuaRnr(const char* luaScript);
	LuaRnr(ITextFileReader* reader,const char* luafilePath);
	~LuaRnr();

public:
	void resetLuaRunner(); /*��������lua�������������ⲿ����*/
	void pushArg(const char* arg); /*�������lua���в����������ⲿ����*/
	bool ready(); /*�����ж�Runner�Ƿ��Ѿ�׼�������У�ֻ�з���trueʱ�ſ��Ե���run*/
	int run(); /*��������lua�ű��������ⲿ����*/

public:
	size_t getArgListSize(); /*���ڻ�ȡlua�ű�ִ�еĲ��������������ⲿ����*/
	size_t getRetvListSize(); /*���ڻ�ȡlua�ű�ִ�еķ���ֵ�����������ⲿ����*/
	const char* getRetvByIdx(size_t idx); /*���ڻ�ȡĳ���±�ķ���ֵ�������ⲿ����*/
	size_t getRetvSizeByIdx(size_t idx); /*���ڻ�ȡĳ���±�ķ���ֵ���ֽ������ݴ�С�������ⲿ����*/
	const char* getScript(); /*���ڻ�ȡ���ص�Runner�еĽű��������ⲿ����*/

public:
	void printScript(); /*��ӡ���ű�����*/
	void printArguments(); /*��ӡ�������б�*/
	void printAllRetvDataInfo(); /*��ӡ�����з���ֵ����Ϣ���������ݴ�С�����ݣ����ַ�������ʽ��ӡ������*/
	void printAllRetvDataByteArray(); /*��ӡ�����з���ֵ����Ϣ���������ݴ�С�����ݣ����ֽ��������ʽ��ӡ������*/

private:
	LuaRnr_imp* m_pImp;
};