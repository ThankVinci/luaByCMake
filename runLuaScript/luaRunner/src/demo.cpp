#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "rlua.h"
#include "LuaRnr.h"
#include "CStandardFileReader.h"

int main()
{
    printf("创建reader前\n");
    ITextFileReader* reader = new CStandardFileReader;
    printf("创建reader后\n");
    LuaRnr* luarnr = new LuaRnr(reader,"./main.lua");
    printf("创建luarnr后\n");
    luarnr->addArg("./entry.json");
    luarnr->addArg("./entry.json");
    luarnr->run();
    luarnr->printRetv();
    return 0;
}