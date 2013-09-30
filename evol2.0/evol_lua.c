#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "evol.h"

/* Function wrapping */

static int l_init(lua_State *L)
{
	evol_init();

	return 0;
}

static int l_quit(lua_State *L)
{
	evol_quit();

	return 0;
}

static int l_get_native_size(lua_State *L)
{
	int w,h;

	evol_get_native_size(&w, &h);

	lua_pushnumber(L, w);
	lua_pushnumber(L, h);

	return 2;
}

static int l_set_mode(lua_State *L)
{
	const char *mode = luaL_checkstring(L, 1);
	int w = luaL_optint(L, 2, 0);
	int h = luaL_optint(L, 3, 0);

	evol_window_mode e_mode;
	if(strcmp(mode, "windowed") == 0)
	{
		e_mode = evol_windowed;
	}
	else if(strcmp(mode, "fullscreen") == 0)
	{
		e_mode = evol_fullscreen;
	}
	else
	{
		luaL_error(L, "invalid window mode: %s\n", mode);
	}

	evol_set_mode(e_mode, w, h);

	return 0;
}

static int l_get_mode(lua_State *L)
{
	evol_window_mode mode;
	int w,h;

	evol_get_mode(&mode, &w, &h);

	switch(mode)
	{
		case evol_windowed:
			lua_pushstring(L, "windowed");
			break;
		case evol_fullscreen:
			lua_pushstring(L, "fullscreen");
			break;
	}

	lua_pushnumber(L, w);
	lua_pushnumber(L, h);

	return 3;
}

static int l_update_timer(lua_State *L)
{
	int fps_cap = luaL_optint(L, 1, 0);
	double dt, fps, t;

	evol_update_timer(fps_cap, &dt, &fps, &t);

	lua_pushnumber(L, dt);
	lua_pushnumber(L, fps);
	lua_pushnumber(L, t);

	return 3;
}

static int l_show_cursor(lua_State *L)
{
	int show = lua_toboolean(L, 1);

	evol_show_cursor(show);

	return 0;
}

static int l_clear_screen(lua_State *L)
{
	int r = luaL_optint(L, 1, 0);
	int g = luaL_optint(L, 2, 0);
	int b = luaL_optint(L, 3, 0);

	evol_clear_screen(r, g, b);

	return 0;
}

static int l_update_screen(lua_State *L)
{
	evol_update_screen();

	return 0;
}

/* Module opening */

static const struct luaL_reg R[] =
{
	{"init", l_init},
	{"quit", l_quit},
	{"get_native_size", l_get_native_size},
	{"set_mode", l_set_mode},
	{"get_mode", l_get_mode},
	{"update_timer", l_update_timer},
	{"show_cursor", l_show_cursor},
	{"clear_screen", l_clear_screen},
	{"update_screen", l_update_screen},
	
	{NULL, NULL}
};

int luaopen_evol (lua_State *L)
{
	luaL_register(L, "evol", R);
	return 1;
}