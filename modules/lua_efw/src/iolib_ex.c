#include "iolib_ex.h"

typedef luaL_Stream LStream;

#define tolstream(L)	((LStream *)luaL_checkudata(L, 1, LUA_FILEHANDLE))

static int io_fclose(lua_State* L) {
	LStream* p = tolstream(L);
	int res = fclose(p->f);
	return luaL_fileresult(L, (res == 0), NULL);
}

static LStream* newprefile(lua_State* L) {
	LStream* p = (LStream*)lua_newuserdatauv(L, sizeof(LStream), 0);
	p->closef = NULL;  /* mark file handle as 'closed' */
	luaL_setmetatable(L, LUA_FILEHANDLE);
	return p;
}

static LStream* newfile(lua_State* L) {
	LStream* p = newprefile(L);
	p->f = NULL;
	p->closef = &io_fclose;
	return p;
}

static int io_open_utf8(lua_State* L) {
	const char* filename = luaL_checkstring(L, 1);
	const char* mode = luaL_optstring(L, 2, "r");
	wchar_t* w_filename = U8StrtoU16Str(filename);
	wchar_t* w_mode = U8StrtoU16Str(mode);
	LStream* p = newfile(L);
	const char* md = mode;  /* to traverse/check mode */
	luaL_argcheck(L, l_checkmode(md), 2, "invalid mode");
	p->f = _wfopen(w_filename, w_mode);
	free(w_filename);
	free(w_mode);
	return (p->f == NULL) ? luaL_fileresult(L, 0, filename) : 1;
}

static int io_pclose(lua_State* L) {
	LStream* p = tolstream(L);
	errno = 0;
	return luaL_execresult(L, _pclose(p->f));
}

static int io_popen_utf8(lua_State* L) {
	const char* filename = luaL_checkstring(L, 1);
	const char* mode = luaL_optstring(L, 2, "r");
	wchar_t* w_filename = U8StrtoU16Str(filename);
	wchar_t* w_mode = U8StrtoU16Str(mode);
	LStream* p = newprefile(L);
	luaL_argcheck(L, l_checkmodep(mode), 2, "invalid mode");
	p->f = _wpopen(w_filename, w_mode);
	p->closef = &io_pclose;
	free(w_filename);
	free(w_mode);
	return (p->f == NULL) ? luaL_fileresult(L, 0, filename) : 1;
}

static const luaL_Reg iolib_funcs[] = {
  {"open", io_open_utf8},
  {"popen", io_popen_utf8},
  {NULL, NULL}
};

int luaload_io_ex(lua_State* L) {
	lua_getglobal(L, LUA_GNAME);
	lua_getfield(L, -1, LUA_IOLIBNAME); 
	if (lua_istable(L, -1)) {
		luaL_setfuncs(L, iolib_funcs, 0);
	}
	else {
		lua_newtable(L);
		luaL_setfuncs(L, iolib_funcs, 0);
		lua_pushstring(L, LUA_OSLIBNAME);
		lua_pushvalue(L, -2);
		lua_setglobal(L, LUA_OSLIBNAME);
	}
	return 0;
}