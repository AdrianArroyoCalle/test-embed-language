#include "lua5.2/lua.hpp"
#include <stdio.h>

int multiplicar(lua_State* vm)
{
	int argc=lua_gettop(vm);
	int a=lua_tointeger(vm,1);
	int b=lua_tointeger(vm,2);
	printf("Multiplicación: %d\n",a*b);
	lua_pushinteger(vm,a*b);
	return 1;
}
int main(int argc, char** argv)
{
	lua_State* vm=luaL_newstate();
	lua_settop(vm,0);
	
	lua_pushcfunction(vm,multiplicar);
	lua_setglobal(vm,"multiplicar");
	
	luaL_loadfile(vm,"test.lua");
	
	lua_pcall(vm,0,LUA_MULTRET,0);
	
	lua_close(vm);
	return 0;
}
