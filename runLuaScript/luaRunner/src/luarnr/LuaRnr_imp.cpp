#include "LuaRnr_imp.h"
#include "rlua.h"
#include "ITextFileReader.hpp"

LuaRnr_imp::LuaRnr_imp(const char* luaScript) {
	addArg(PROGRAM_NAME);
	addArg(luaScript);
}
LuaRnr_imp::LuaRnr_imp(ITextFileReader* reader, const char* luafilePath)
{
	addArg(PROGRAM_NAME);
	if (reader->readAll(luafilePath) == EXIT_SUCCESS) {
		const char* content = reader->getData();
		addArg(content);
	}
}
void LuaRnr_imp::addArg(const char* arg)
{
	argList.push_back(arg);
}

int LuaRnr_imp::run()
{
	int retc;
	char** retv;
	const char** argv = packArgsList();
	int result = runLua(argList.size(), (char**)argv, &retc, &retv);
	for (int i = 0; i < retc; i++) {
		retList.push_back(retv[i]);
	}
	freeArgs(argv);
	return result;
}

char* LuaRnr_imp::getIdx(size_t idx)
{
	return nullptr;
}

size_t LuaRnr_imp::getRetvSize()
{
	return size_t();
}

void LuaRnr_imp::printRetv()
{
	for (char* s : retList) {
		printf("%s\n", s);
	}
}

const char** LuaRnr_imp::packArgsList()
{
	const char** args = (const char**)malloc(sizeof(char*) * argList.size());
	for (size_t i = 0; i != argList.size(); i++) {
		args[i] = argList[i].c_str();
	}
	return args;
}

void LuaRnr_imp::freeArgs(const char** args)
{
	free(args);
}

LuaRnr_imp::~LuaRnr_imp()
{
}