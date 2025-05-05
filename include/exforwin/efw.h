#ifndef _EFW_H
#define _EFW_H

//标准库
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//lua 库
#include "lprefix.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

//Windows专属
#ifdef _WIN32
#include <windows.h>
#include <wchar.h>
#define WIN_UTF8 1
#endif

#ifdef WIN_UTF8
#include <fcntl.h>
static wchar_t* U8StrtoU16Str(const char* u8_str) {
    size_t u8_len = strlen(u8_str);
    size_t u16_len = MultiByteToWideChar(CP_UTF8, 0, u8_str, u8_len, NULL, 0);
    wchar_t* u16_str = (wchar_t*)malloc((u16_len + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, u8_str, -1, u16_str, u16_len); //转为宽字符串即（UTF-16）
    u16_str[u16_len] = L'\0';
    return u16_str;
}

static char* U16StrtoU8Str(wchar_t* u16_str) {
    size_t u16_len = wcslen(u16_str);
    size_t u8_len = WideCharToMultiByte(CP_UTF8, 0, u16_str, u16_len, NULL, 0, NULL, NULL);
    char* u8_str = (char*)malloc((u8_len + 1) * sizeof(char));
    WideCharToMultiByte(CP_UTF8, 0, u16_str, -1, u8_str, u8_len, NULL, NULL); //转为UTF-8窄字符串
    u8_str[u8_len] = '\0';
    return u8_str;
}

/* 实现宽字符串输出,是lua_writestring的宽字符串版本
目前只在luaB_print_utf8中使用这个函数实现输出，进行输出前，需要先设置翻译模式为_O_U16TEXT
输出完成，刷新到文件流后需要把翻译模式改回_O_TEXT，避免一些情况下，lua原生使用fputs等接口输出导致错误
*/

#if !defined(lua_writewstring) 
#if USE_MSVCRT_ == 1 // 如果使用mingw-msvcrt进行编译的话，fwrite是无法成功输出宽字符串的
// 使用 MSVCRT
static size_t lua_writewstring(const wchar_t* s,const size_t l) {
    for (size_t i = 0; i < l; i += 1)
        fputwc(s[i], stdout); 
    return l;
}
#else
#define lua_writewstring(s,l)  fwrite((s), sizeof(wchar_t), (l), stdout)
#endif
#endif // lua_writewstring

/* 打印宽字符串换行并完成刷新 */
#if !defined(lua_writewline)
#define lua_writewline()        (lua_writewstring(L"\n", 1), fflush(stdout))
#endif // lua_writewline


#endif // WIN_UTF8

#endif //_EFW_H