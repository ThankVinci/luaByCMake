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

//����ע���ģ������
#define BASE_LIB_EX "base_ex" //baseģ�����չ
#define OS_LIB_EX "os_ex" //osģ�����չ

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
#endif

#endif //_EFW_H