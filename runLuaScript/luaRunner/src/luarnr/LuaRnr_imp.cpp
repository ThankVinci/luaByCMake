#include "rlua.h"
#include "luarnr/LuaRnr_imp.h"
#include "io/ITextFileReader.hpp"

LuaRnr_imp::LuaRnr_imp(const char* luaScript) 
{
	initOnce();
	saveScript(luaScript);
	resetLuaRunner(); //��ʼ��lua������
}

LuaRnr_imp::LuaRnr_imp(ITextFileReader* reader, const char* luafilePath)
{
	initOnce();
	if (reader != nullptr) {
		if (reader->readAll(luafilePath) == EXIT_SUCCESS) {
			//�������EXIT_SUCCESS�Ļ���bufferһ������NULL
			//���Ƕ�ȡ���Ľű��Ƿ���Ч���᲻�������ֻ���ڽű��н��п���
			const char* luaScript = reader->getData(); 
			//�����ȡ����data�洢��reader�еĳ�Ա�У���reader�������������Ѿ������˿ռ䣬���Բ���Ҫ�ֶ�����
			saveScript(luaScript); //����ű��ַ�������һ�ݿ���
		}
		//�ڶ��߳�����reader������������
		//reader�����һ����������ָ�룬Ȼ����������յ�������reader�е�bufferҲ����յ�
		delete reader; 
	}
	resetLuaRunner();
}

LuaRnr_imp::~LuaRnr_imp()
{
	clearArgList(); /*��ղ����б�*/
	clearRetValList(); /*��շ���ֵ�б�*/
	clearRetSizeList(); /*��շ���ֵ��С�б�*/
	m_sScript = "";
	m_bLoad = false;
}

void LuaRnr_imp::resetLuaRunner() 
{
	initRunner();
	loadScript();
}

void LuaRnr_imp::pushArg(string& arg) 
{
	m_argList.push_back(arg);
}

bool LuaRnr_imp::ready()
{
	return m_bLoad;
}

int LuaRnr_imp::run() 
{
	int retc = 0;
	char** retv = NULL;
	unsigned long long* rets = NULL;
	const char** argv = packArgs();
	int result = runLua((int)m_argList.size(), argv, &retc, &retv, &rets);
	for (int i = 0; i < retc; i++) {
		m_retValList.push_back(retv[i]);
		m_retSizeList.push_back(rets[i]);
	}
	freeArgs(argv); //�ͷ��ڴ�
	return result;
}

size_t LuaRnr_imp::getArgListSize()
{
	return m_argList.size();
}

size_t LuaRnr_imp::getRetvListSize()
{
	return m_retValList.size();
}

const char* LuaRnr_imp::getRetvByIdx(size_t idx)
{
	char* retv = nullptr;
	if (getRetvListSize() > 0) {
		size_t rets = m_retSizeList[idx];
		retv = (char*)malloc(sizeof(char*) * rets);
		memcpy(retv, m_retValList[idx], rets); //Ϊ�˱�֤��ȫ�Ի�ȡ����ʱҪ����һ�����ݷ���
	}
	return retv;
}

size_t LuaRnr_imp::getRetvSizeByIdx(size_t idx)
{
	if (getRetvListSize() > 0) {
		return m_retSizeList[idx];
	}
	return 0;
}

const string& LuaRnr_imp::getScript()
{
	return m_sScript;
}

void LuaRnr_imp::initOnce() 
{
	m_sScript = "";
}

void LuaRnr_imp::initRunner() 
{
	m_bLoad = false;
	clearArgList(); /*��ղ����б�*/
	clearRetValList(); /*��շ���ֵ�б�*/
	clearRetSizeList(); /*��շ���ֵ��С�б�*/
	string runnername = PROGRAM_NAME;
	pushArg(runnername);
}

void LuaRnr_imp::loadScript() 
{
	if (!m_bLoad) { //һ������initRunner֮��ִ�У���Ϊ�ű�ֻ�ܼ���һ��
		pushArg(m_sScript);
		m_bLoad = true;
	}
}

void LuaRnr_imp::saveScript(string script) 
{
	m_sScript = script;
}

const char** LuaRnr_imp::packArgs()
{
	if (getArgListSize() > 0) {
		const char** args = (const char**)malloc(sizeof(char*) * m_argList.size());
		for (size_t i = 0; i != m_argList.size(); i++) {
			args[i] = m_argList[i].c_str();
		}
		return args;
	}
	return NULL;
}

void LuaRnr_imp::freeArgs(const char** args)
{
	if (args != NULL) {
		free(args);
	}
}

void LuaRnr_imp::clearArgList()
{
	m_argList.clear();
}

void LuaRnr_imp::clearRetValList()
{
	for (const char* s : m_retValList) {
		if(s != NULL) free((void*)s);
	}
	m_retValList.clear();
}

void LuaRnr_imp::clearRetSizeList()
{
	m_retSizeList.clear();
}

/*******************��ӡ��Ϣ***************************/
void LuaRnr_imp::printScript()
{
	printf("%s\n",m_sScript.c_str());
}
void LuaRnr_imp::printArguments()
{
	printf("arguments:[");
	for (size_t idx = 2; idx < m_argList.size();idx++) {
		if (idx < m_argList.size()-1) {
			printf("%s,",m_argList[idx].c_str());
		}
		else {
			printf("%s", m_argList[idx].c_str());
		}
	}
	printf("]\n");
}
void LuaRnr_imp::printAllRetvDataInfo()
{
	for (size_t idx = 0; idx < m_retValList.size();idx++) {
		printf("[dataSize:%zd,", m_retSizeList[idx]);
		printf("data:%s ]\n",m_retValList[idx]);
	}
}
void LuaRnr_imp::printAllRetvDataByteArray()
{
	auto printByteArray = [](const char* byteArray,size_t size) {
		printf("data:[ " );
		for (size_t idx = 0; idx < size;idx++) {
			printf("0x%.2x ",(unsigned char)byteArray[idx]);
		}
		printf("]");
	};
	for (size_t idx = 0; idx < m_retValList.size(); idx++) {
		printf("{idx:%zd,",idx);
		printf("dataSize:%zd,", m_retSizeList[idx]);
		printByteArray(m_retValList[idx], m_retSizeList[idx]);
		printf("}\n");
	}
}
/******************************************************/
