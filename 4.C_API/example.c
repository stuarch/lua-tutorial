#include <stdio.h>
#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

static int l_hello(lua_State *L){
    const char *name = lua_tostring(L, 1);
    lua_pop(L, 1);
    printf("Hello %s\n", name);
    return 0;
}

static int l_total(lua_State *L){
    double i = lua_tonumber(L, 1);
    double j = lua_tonumber(L, 2);
    lua_pop(L, 2);
    lua_pushnumber(L, i+j);
    return 1;
}

static const struct luaL_Reg example[] = {
    {"hello", l_hello},
    {"total", l_total},
    {NULL, NULL},
};

int luaopen_example(lua_State *L){
    lua_newtable(L);
    luaL_register(L, NULL, example);
    return 1;
}
