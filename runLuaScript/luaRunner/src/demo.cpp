#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "luarnr/LuaRnr.h"
#include "io/CStandardFileReader.h"

int main()
{
    LuaRnr* luarnr = new LuaRnr(
        new CStandardFileReader, 
        "C:\\Path\\Codelib\\luamodules\\script\\main.lua");
    //LuaRnr* luarnr = new LuaRnr(" ");
    luarnr->pushArg("C:\\Path\\Codelib\\test\\entry.json");
    printf("argsize:%zd\n", luarnr->getArgListSize()); //打印传入脚本的参数数量
    luarnr->printArguments();
    int result = luarnr->run();
    printf("retvsize:%zd\n", luarnr->getRetvListSize());
    luarnr->printAllRetvDataInfo();
    luarnr->printAllRetvDataByteArray();
    printf("script:%s\n", luarnr->getScript());
    delete luarnr;
    return 0;
}