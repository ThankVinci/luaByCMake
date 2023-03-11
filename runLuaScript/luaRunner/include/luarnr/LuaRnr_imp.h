#include <string>
#include <vector>
using std::string;
using std::vector;

class ITextFileReader;

#define PROGRAM_NAME "LuaRunner"
class  LuaRnr_imp {
private:
public:
	LuaRnr_imp(const char* luaScript);
	LuaRnr_imp(ITextFileReader* reader, const char* luafilePath);

	void addArg(const char* arg);

	int run();

	void freeArgs(const char** args);

	char* getIdx(size_t idx);

	size_t getRetvSize();

	void printRetv();

	~LuaRnr_imp();

private:
	const char** packArgsList();

private:
	vector<string> argList;

	vector<char*> retList;
};