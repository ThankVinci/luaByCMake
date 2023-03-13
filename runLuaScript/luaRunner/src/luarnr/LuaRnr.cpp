#include "luarnr/LuaRnr.h"
#include "luarnr/LuaRnr_imp.h"

LuaRnr::LuaRnr(const char* luaScript) {
	m_pImp = new LuaRnr_imp(luaScript);
}

LuaRnr::LuaRnr(ITextFileReader* reader, const char* luafilePath)
{
	m_pImp = new LuaRnr_imp(reader, luafilePath);
}

LuaRnr::~LuaRnr()
{
	if (m_pImp != nullptr) delete m_pImp;
}

void LuaRnr::resetLuaRunner()
{
	m_pImp->resetLuaRunner();
}

void LuaRnr::pushArg(const char* arg)
{
	string sArg = arg;
	m_pImp->pushArg(sArg);
}

bool LuaRnr::ready()
{
	return m_pImp->ready();
}

int LuaRnr::run()
{
	return m_pImp->run();
}

size_t LuaRnr::getArgListSize()
{
	return m_pImp->getArgListSize();
}

size_t LuaRnr::getRetvListSize()
{
	return m_pImp->getRetvListSize();
}

const char* LuaRnr::getRetvByIdx(size_t idx)
{
	return m_pImp->getRetvByIdx(idx);
}

size_t LuaRnr::getRetvSizeByIdx(size_t idx)
{
	return m_pImp->getRetvSizeByIdx(idx);
}

const char* LuaRnr::getScript()
{
	return m_pImp->getScript().c_str();
}

void LuaRnr::printScript()
{
	m_pImp->printScript();
}

void LuaRnr::printArguments()
{
	m_pImp->printArguments();
}

void LuaRnr::printCommand()
{
	m_pImp->printCommand();
}

void LuaRnr::printAllRetvDataInfo()
{
	m_pImp->printAllRetvDataInfo();
}
void LuaRnr::printAllRetvDataByteArray()
{
	m_pImp->printAllRetvDataByteArray();
}