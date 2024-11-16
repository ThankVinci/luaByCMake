#include "loadlib_ex.h"

static void findloader(lua_State* L, const char* name) {
    int i;
    luaL_Buffer msg;  /* to build error message */
    /* push 'package.searchers' to index 3 in the stack */
    if (l_unlikely(lua_getfield(L, lua_upvalueindex(1), "searchers")
        != LUA_TTABLE))
        luaL_error(L, "'package.searchers' must be a table");
    luaL_buffinit(L, &msg);
    /*  iterate over available searchers to find a loader */
    for (i = 1; ; i++) {
        luaL_addstring(&msg, "\n\t");  /* error-message prefix */
        if (l_unlikely(lua_rawgeti(L, 3, i) == LUA_TNIL)) {  /* no more searchers? */
            lua_pop(L, 1);  /* remove nil */
            luaL_buffsub(&msg, 2);  /* remove prefix */
            luaL_pushresult(&msg);  /* create error message */
            luaL_error(L, "module '%s' not found:%s", name, lua_tostring(L, -1));
        }
        lua_pushstring(L, name);
        lua_call(L, 1, 2);  /* call it */
        if (lua_isfunction(L, -2))  /* did it find a loader? */
            return;  /* module loader found */
        else if (lua_isstring(L, -2)) {  /* searcher returned error message? */
            lua_pop(L, 1);  /* remove extra return */
            luaL_addvalue(&msg);  /* concatenate error message */
        }
        else {  /* no error message */
            lua_pop(L, 2);  /* remove both returns */
            luaL_buffsub(&msg, 2);  /* remove prefix */
        }
    }
}

static int ll_require_utf8(lua_State* L) {
    const char* name = luaL_checkstring(L, 1);
    lua_settop(L, 1);  /* LOADED table will be at index 2 */
    lua_getfield(L, LUA_REGISTRYINDEX, LUA_LOADED_TABLE);
    lua_getfield(L, 2, name);  /* LOADED[name] */
    if (lua_toboolean(L, -1))  /* is it there? */
        return 1;  /* package is already loaded */
    /* else must load package */
    lua_pop(L, 1);  /* remove 'getfield' result */
    findloader(L, name);
    lua_rotate(L, -2, 1);  /* function <-> loader data */
    lua_pushvalue(L, 1);  /* name is 1st argument to module loader */
    lua_pushvalue(L, -3);  /* loader data is 2nd argument */
    /* stack: ...; loader data; loader function; mod. name; loader data */
    lua_call(L, 2, 1);  /* run loader to load module */
    /* stack: ...; loader data; result from loader */
    if (!lua_isnil(L, -1))  /* non-nil return? */
        lua_setfield(L, 2, name);  /* LOADED[name] = returned value */
    else
        lua_pop(L, 1);  /* pop nil */
    if (lua_getfield(L, 2, name) == LUA_TNIL) {   /* module set no value? */
        lua_pushboolean(L, 1);  /* use true as result */
        lua_copy(L, -1, -2);  /* replace loader result */
        lua_setfield(L, 2, name);  /* LOADED[name] = true */
    }
    lua_rotate(L, -2, 1);  /* loader data <-> module result  */
    return 2;  /* return module result and loader data */
}

static const luaL_Reg loadlib_funcs[] = {
  {"require", ll_require_utf8},
  {NULL, NULL}
};

int luaload_load_ex(lua_State* L) {
    lua_getglobal(L, LUA_GNAME);
    luaL_setfuncs(L, loadlib_funcs, 1);
    return 0;
}