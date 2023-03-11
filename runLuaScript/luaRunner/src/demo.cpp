#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "rlua.h"
#include "LuaRnr.h"
#include "CStandardFileReader.h"

int main()
{

    ITextFileReader* reader = new CStandardFileReader;

    LuaRnr* luarnr = new LuaRnr(reader,".\\main.lua");
    luarnr->addArg("C:\\Projects\\CMakeProjs\\LuaRunnerByCMake\\out\\build\\x64-Release\\bin\\entry.json");
    luarnr->addArg("C:\\Projects\\CMakeProjs\\LuaRunnerByCMake\\out\\build\\x64-Release\\bin\\entry.json");
    luarnr->run();
    luarnr->printRetv();
    return 0;
}