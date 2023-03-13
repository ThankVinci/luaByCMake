#include <stddef.h>
#include <string>
#include <vector>
using std::string;
using std::vector;

class ITextFileReader;

#define PROGRAM_NAME "LuaRunner"
class LuaRnr_imp {

public: //构造和析构
	LuaRnr_imp(const char* luaScript);
	LuaRnr_imp(ITextFileReader* reader, const char* luafilePath);
	~LuaRnr_imp();

public:
	void resetLuaRunner(); /*用于重置lua运行器，可在外部调用*/
	void pushArg(string& arg); /*用于添加lua运行参数，可在外部调用*/
	bool ready(); /*用于判断Runner是否已经准备好运行，判断依据是load成员*/
	int run(); /*用于运行lua脚本，可在外部调用*/

public:
	size_t getArgListSize(); /*用于获取lua脚本执行的参数数量，可在外部调用*/
	size_t getRetvListSize(); /*用于获取lua脚本执行的返回值数量，可在外部调用*/
	const char* getRetvByIdx(size_t idx); /*用于获取某个下标的返回值，可在外部调用*/
	size_t getRetvSizeByIdx(size_t idx); /*用于获取某个下标的返回值的字节流数据大小，可在外部调用*/
	const string& getScript(); /*用于获取加载到Runner中的脚本，可在外部调用*/

private:
	void initOnce(); /*lua运行器中只需要初始化一次的变量，不可在外部调用*/
	void initRunner(); /*初始化lua运行器，不可在外部调用*/
	void loadScript(); /*把脚本加载到参数列表中，不可在外部调用*/
	void saveScript(string script); /*将脚本保存到成员中，只会在构造函数中调用，不可在外部调用*/

private:
	const char** packArgs(); /*在run中进行runLua时包装m_argList为二维数组，不可在外部调用*/
	void freeArgs(const char** args); /*在run中进行runLua之后回收作为参数列表的二维数组，不可在外部调用*/
	void clearArgList(); /*清空参数列表，不可在外部调用*/
	void clearRetValList(); /*清空返回值列表，不可在外部调用*/
	void clearRetSizeList(); /*清空返回值大小列表，不可在外部调用*/

public:
	void printScript(); /*打印出脚本内容*/
	void printArguments(); /*打印出参数列表*/
	void printAllRetvDataInfo(); /*打印出所有返回值的信息，包括数据大小和数据（以字符串的形式打印出来）*/
	void printAllRetvDataByteArray(); /*打印出所有返回值的信息，包括数据大小和数据（以字节数组的形式打印出来）*/

private:
	vector<string> m_argList; /*存放运行参数列表*/
	vector<const char*> m_retValList; /*存放返回值列表*/
	vector<unsigned long long> m_retSizeList; /*存放返回值大小列表，这个大小指的是字节流数据的大小，如果接收的是字符串数据的话，就不用管*/
	string m_sScript; /*存放lua脚本字符串*/
	bool m_bLoad; /*表示是否已经加载了脚本字符串*/
};