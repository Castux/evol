#ifndef evol_lua_h
#define evol_lua_h

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "evol.h"

int luaopen_evol (lua_State *L);

#endif