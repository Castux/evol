#include <stdio.h>

#include "evol_lua.h"

extern int luaopen_evol(lua_State *L);

int main(int argc, char **argv)
{
	lua_State *L;
	char *path;
	
	if (argc < 2)
	{
		path = "main.lua";
	}
	else
	{
		path = argv[1];
	}
	
	printf("Using file %s\n", path);
	
	L = luaL_newstate();
	luaL_openlibs(L);
	
	luaL_requiref(L, "evol", luaopen_evol, 1);

	if (luaL_dofile(L, path) != 0)
	{
		printf("%s\n",lua_tostring(L,-1));
	}
		
	lua_close(L);
	
	return 0;
}
