#include "demo.h"

static int demo_hello(lua_State* L) {
	printf("hello,demo\n\r");
	return 0;
}

static const struct luaL_Reg demolib[] = {
	{"demo_hello",demo_hello},
	{NULL,NULL}
};

LUALIB_API int luaopen_demo(lua_State* L) {
	//luaL_register(L, demo_hello);
	luaL_newlib(L, demolib);
	printf("wtf\n\r");
	return 1;
}