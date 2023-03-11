#include "LuaRnr.h"
#include "LuaRnr_imp.h"

LuaRnr::LuaRnr(const char* luaScript) {
	m_pImp = new LuaRnr_imp(luaScript);
}
LuaRnr::LuaRnr(ITextFileReader* reader, const char* luafilePath)
{
	m_pImp = new LuaRnr_imp(reader, luafilePath);
}
void LuaRnr::addArg(const char* arg)
{
	m_pImp->addArg(arg);
}

int LuaRnr::run()
{
	return m_pImp->run();
}

void LuaRnr::printRetv()
{
	m_pImp->printRetv();
}

LuaRnr::~LuaRnr()
{
	if (m_pImp != nullptr) delete m_pImp;
}