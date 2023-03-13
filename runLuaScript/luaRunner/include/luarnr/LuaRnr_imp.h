#include <stddef.h>
#include <string>
#include <vector>
using std::string;
using std::vector;

class ITextFileReader;

#define PROGRAM_NAME "LuaRunner"
class LuaRnr_imp {

public: //���������
	LuaRnr_imp(const char* luaScript);
	LuaRnr_imp(ITextFileReader* reader, const char* luafilePath);
	~LuaRnr_imp();

public:
	void resetLuaRunner(); /*��������lua�������������ⲿ����*/
	void pushArg(string& arg); /*�������lua���в����������ⲿ����*/
	bool ready(); /*�����ж�Runner�Ƿ��Ѿ�׼�������У��ж�������load��Ա*/
	int run(); /*��������lua�ű��������ⲿ����*/

public:
	size_t getArgListSize(); /*���ڻ�ȡlua�ű�ִ�еĲ��������������ⲿ����*/
	size_t getRetvListSize(); /*���ڻ�ȡlua�ű�ִ�еķ���ֵ�����������ⲿ����*/
	const char* getRetvByIdx(size_t idx); /*���ڻ�ȡĳ���±�ķ���ֵ�������ⲿ����*/
	size_t getRetvSizeByIdx(size_t idx); /*���ڻ�ȡĳ���±�ķ���ֵ���ֽ������ݴ�С�������ⲿ����*/
	const string& getScript(); /*���ڻ�ȡ���ص�Runner�еĽű��������ⲿ����*/

private:
	void initOnce(); /*lua��������ֻ��Ҫ��ʼ��һ�εı������������ⲿ����*/
	void initRunner(); /*��ʼ��lua���������������ⲿ����*/
	void loadScript(); /*�ѽű����ص������б��У��������ⲿ����*/
	void saveScript(string script); /*���ű����浽��Ա�У�ֻ���ڹ��캯���е��ã��������ⲿ����*/

private:
	const char** packArgs(); /*��run�н���runLuaʱ��װm_argListΪ��ά���飬�������ⲿ����*/
	void freeArgs(const char** args); /*��run�н���runLua֮�������Ϊ�����б�Ķ�ά���飬�������ⲿ����*/
	void clearArgList(); /*��ղ����б��������ⲿ����*/
	void clearRetValList(); /*��շ���ֵ�б��������ⲿ����*/
	void clearRetSizeList(); /*��շ���ֵ��С�б��������ⲿ����*/

public:
	void printScript(); /*��ӡ���ű�����*/
	void printArguments(); /*��ӡ�������б�*/
	void printAllRetvDataInfo(); /*��ӡ�����з���ֵ����Ϣ���������ݴ�С�����ݣ����ַ�������ʽ��ӡ������*/
	void printAllRetvDataByteArray(); /*��ӡ�����з���ֵ����Ϣ���������ݴ�С�����ݣ����ֽ��������ʽ��ӡ������*/

private:
	vector<string> m_argList; /*������в����б�*/
	vector<const char*> m_retValList; /*��ŷ���ֵ�б�*/
	vector<unsigned long long> m_retSizeList; /*��ŷ���ֵ��С�б������Сָ�����ֽ������ݵĴ�С��������յ����ַ������ݵĻ����Ͳ��ù�*/
	string m_sScript; /*���lua�ű��ַ���*/
	bool m_bLoad; /*��ʾ�Ƿ��Ѿ������˽ű��ַ���*/
};