#ifndef _LIBEX4WIN_H
#define _LIBEX4WIN_H

#include "lprefix.h"

#include <errno.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#ifdef _WIN32
#include <windows.h>
#define WIN_UTF8 1
#endif

#ifdef WIN_UTF8
#include <fcntl.h>
static wchar_t* U8StrtoU16Str(const char* u8_str) {
    size_t u8_len = strlen(u8_str);
    size_t u16_len = MultiByteToWideChar(CP_UTF8, 0, u8_str, u8_len, NULL, 0);
    wchar_t* u16_str = malloc((u16_len + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, u8_str, -1, u16_str, u16_len); //转为宽字符串即（UTF-16）
    u16_str[u16_len] = L'\0';
    return u16_str;
}

static char* U16StrtoU8Str(wchar_t* u16_str) {
    size_t u16_len = wcslen(u16_str);
    size_t u8_len = WideCharToMultiByte(CP_UTF8, 0, u16_str, u16_len, NULL, 0, NULL, NULL);
    char* u8_str = malloc((u8_len + 1) * sizeof(char));
    WideCharToMultiByte(CP_UTF8, 0, u16_str, -1, u8_str, u8_len, NULL, NULL); //转为UTF-8窄字符串
    u8_str[u8_len] = '\0';
    return u8_str;
}
#endif

#define LUA_EFW_BASE "base_ex" //efw的全局
#define LUA_EFW_OS "os_ex" //

LUALIB_API int (luaopen_base_ex)(lua_State* L);

LUALIB_API int (luaopen_os_ex)(lua_State* L);

#endif //_LIBEX4WIN_H