#ifndef _EFW_H
#define _EFW_H

//��׼��
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//lua ��
#include "lprefix.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

//Windowsר��
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
    MultiByteToWideChar(CP_UTF8, 0, u8_str, -1, u16_str, u16_len); //תΪ���ַ�������UTF-16��
    u16_str[u16_len] = L'\0';
    return u16_str;
}

static char* U16StrtoU8Str(wchar_t* u16_str) {
    size_t u16_len = wcslen(u16_str);
    size_t u8_len = WideCharToMultiByte(CP_UTF8, 0, u16_str, u16_len, NULL, 0, NULL, NULL);
    char* u8_str = (char*)malloc((u8_len + 1) * sizeof(char));
    WideCharToMultiByte(CP_UTF8, 0, u16_str, -1, u8_str, u8_len, NULL, NULL); //תΪUTF-8խ�ַ���
    u8_str[u8_len] = '\0';
    return u8_str;
}

/* ʵ�ֿ��ַ������,��lua_writestring�Ŀ��ַ����汾
Ŀǰֻ��luaB_print_utf8��ʹ���������ʵ��������������ǰ����Ҫ�����÷���ģʽΪ_O_U16TEXT
�����ɣ�ˢ�µ��ļ�������Ҫ�ѷ���ģʽ�Ļ�_O_TEXT������һЩ����£�luaԭ��ʹ��fputs�Ƚӿ�������´���
*/

#if !defined(lua_writewstring) 
#if USE_MSVCRT_ == 1 // ���ʹ��mingw-msvcrt���б���Ļ���fwrite���޷��ɹ�������ַ�����
// ʹ�� MSVCRT
static size_t lua_writewstring(const wchar_t* s,const size_t l) {
    for (size_t i = 0; i < l; i += 1)
        fputwc(s[i], stdout); 
    return l;
}
#else
#define lua_writewstring(s,l)  fwrite((s), sizeof(wchar_t), (l), stdout)
#endif
#endif // lua_writewstring

/* ��ӡ���ַ������в����ˢ�� */
#if !defined(lua_writewline)
#define lua_writewline()        (lua_writewstring(L"\n", 1), fflush(stdout))
#endif // lua_writewline


#endif // WIN_UTF8

#endif //_EFW_H