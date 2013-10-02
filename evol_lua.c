#include "evol_lua.h"

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

	lua_pushinteger(L, w);
	lua_pushinteger(L, h);

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

	lua_pushinteger(L, w);
	lua_pushinteger(L, h);

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

static int l_load_image(lua_State *L)
{
	const char *path = luaL_checkstring(L, 1);

	evol_image image = evol_load_image(path);

	lua_pushlightuserdata(L, image);

	return 1;
}

static evol_image check_image(lua_State *L)
{
	evol_image image = (evol_image) lua_touserdata(L, 1);

	if(!image)
	{
		luaL_error(L, "invalid image\n");
	}

	return image;
}

static int l_free_image(lua_State *L)
{
	evol_image image = check_image(L);

	evol_free_image(image);

	return 0;
}

static int l_get_image_size(lua_State *L)
{
	evol_image image = check_image(L);
	int w,h;

	evol_get_image_size(image, &w, &h);

	lua_pushinteger(L, w);
	lua_pushinteger(L, h);

	return 2;
}

static int l_set_image_modulation(lua_State *L)
{
	evol_image image = check_image(L);
	
	int r = luaL_optint(L, 2, 255);
	int g = luaL_optint(L, 3, 255);
	int b = luaL_optint(L, 4, 255);
	int a = luaL_optint(L, 5, 255);

	evol_set_image_modulation(image, r, g, b, a);

	return 0;
}

static int l_draw_image(lua_State *L)
{
	evol_image image = check_image(L);
	
	int x = luaL_checkint(L, 2);
	int y = luaL_checkint(L, 3);

	if(lua_gettop(L) > 3)
	{
		double scale_x = luaL_optnumber(L, 4, 1.0);
		double scale_y = luaL_optnumber(L, 5, 1.0);

		int pivot_x = luaL_optint(L, 6, 0);
		int pivot_y = luaL_optint(L, 7, 0);

		double angle = luaL_optnumber(L, 8, 0.0);

		evol_draw_image2(image, x, y, scale_x, scale_y, pivot_x, pivot_y, angle);
	}
	else
	{
		evol_draw_image(image, x, y);
	}

	return 0;
}

static const char *event_devices[] =
{
	"mouse",
	"keyboard",
	"gamepad",
	"system"
};

static const char *event_types[] =
{
	"button",
	"axis",
	"quit"
};

static const char *gamepad_axes[] =
{
	"left",
	"right",
	"triggers"
};

static const char *gamepad_buttons[] =
{
	"A",
	"B",
	"X",
	"Y",
	"back",
	"guide",
	"start",
	"left stick",
	"right stick",
	"left shoulder",
	"right shoulder",
	"up",
	"down",
	"left",
	"right"
};

static const char *mouse_axes[] =
{
	"motion",
	"wheel"
};

static int l_input(lua_State *L)
{
	evol_event e;

	if(!evol_input(&e))
	{
		return 0;
	}

	if(!lua_istable(L, 1))
	{
		lua_newtable(L);
	}

	lua_pushstring(L, event_devices[e.device]);
	lua_setfield(L, -2, "device");

	lua_pushstring(L, event_types[e.type]);
	lua_setfield(L, -2, "type");

	if(e.device == evol_gamepad)
	{
		lua_pushinteger(L, e.device_num);
		lua_setfield(L, -2, "device_num");

		switch(e.type)
		{
			case evol_axis_event:
				lua_pushstring(L, gamepad_axes[e.elem_num]);
				lua_setfield(L, -2, "axis");
				break;
			case evol_button_event:
				lua_pushstring(L, gamepad_buttons[e.elem_num]);
				lua_setfield(L, -2, "button");
				break;
		}
	}
	else if(e.device == evol_mouse)
	{
		switch(e.type)
		{
			case evol_axis_event:
				lua_pushstring(L, mouse_axes[e.elem_num]);
				lua_setfield(L, -2, "axis");
				break;
			case evol_button_event:
				lua_pushinteger(L, e.elem_num);
				lua_setfield(L, -2, "button");
				break;
		}
	}
	else if(e.device == evol_keyboard)
	{
		lua_pushstring(L, evol_get_key_name(e.elem_num));
		lua_setfield(L, -2, "key");
	}

	if(e.type == evol_axis_event)
	{
		lua_pushnumber(L, e.x);
		lua_setfield(L, -2, "x");
		lua_pushnumber(L, e.y);
		lua_setfield(L, -2, "y");
	}
	else if(e.type == evol_button_event)
	{
		lua_pushstring(L, e.state == evol_up ? "up" : "down");
		lua_setfield(L, -2, "state");
	}

	return 1;
}

static int l_load_font(lua_State *L)
{
	const char *path = luaL_checkstring(L, 1);
	int size = luaL_optinteger(L, 2, 12);

	evol_font font = evol_load_font(path, size);

	lua_pushlightuserdata(L, font);

	return 1;
}

static evol_font check_font(lua_State *L)
{
	evol_font font = (evol_font) lua_touserdata(L, 1);

	if(!font)
	{
		luaL_error(L, "invalid font\n");
	}

	return font;
}

static int l_free_font(lua_State *L)
{
	evol_font font = check_font(L);

	evol_free_font(font);

	return 0;
}

static int l_draw_font(lua_State *L)
{
	evol_font font = check_font(L);
	const char *text = luaL_checkstring(L, 2);
	int r = luaL_optinteger(L, 3 , 0);
	int g = luaL_optinteger(L, 4 , 0);
	int b = luaL_optinteger(L, 5 , 0);

	evol_image image = evol_draw_font(font, text, r, g, b);

	lua_pushlightuserdata(L, image);

	return 1;
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

	{"load_image", l_load_image},
	{"free_image", l_free_image},
	{"get_image_size", l_get_image_size},
	{"set_image_modulation", l_set_image_modulation},
	{"draw_image", l_draw_image},

	{"load_font", l_load_font},
	{"free_font", l_free_font},
	{"draw_font", l_draw_font},

	{"input", l_input},
	
	{NULL, NULL}
};

int luaopen_evol (lua_State *L)
{
	luaL_register(L, "evol", R);
	return 1;
}