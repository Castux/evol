/*
 * evol_dingux.c -- evöL
 * Copyright (C) 2010 Noé Falzon
 *
 * This program is  free software:  you can redistribute it and/or modify it
 * under  the terms  of the  GNU General Public License  as published by the
 * Free Software Foundation, version 3 of the License.
 *
 * This program  is  distributed  in the hope  that it will  be useful,  but
 * WITHOUT  ANY WARRANTY;  without  even the implied  warranty of MERCHANTA-
 * BILITY  or  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 * License for more details.
 *
 * You should have received a copy of the  GNU General Public License  along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_gfxPrimitives.h"
#include "SDL_mixer.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

/* evöL stuff */

extern SDL_Surface *gScreen;
extern int sound_on;

extern int luaopen_evolcore (lua_State *L);

/* Prototypes */

void init_SDL(void);
void quit_SDL(void);

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
	
	init_SDL();
	
	L = luaL_newstate();
	luaL_openlibs(L);
	
	lua_pushcfunction(L, luaopen_evolcore);
	lua_call(L,0,0);

	if (luaL_dostring(L,"evolcore.init = function() end; evolcore.quit = function() end") != 0)
		printf("%s\n",lua_tostring(L,-1));
	
	if (luaL_dofile(L, path)!=0)
		printf("%s\n",lua_tostring(L,-1));
		
	lua_close(L);
	quit_SDL();
	
	return 0;
}


void init_SDL(void)
{
	printf("evöL init, Dingoo mode\n");
	
	/* Init SDL */
		
	if( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0 )
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	
	printf("SDL initialized\n");
	
	gScreen=SDL_SetVideoMode(320,240,16,SDL_SWSURFACE);

	if(gScreen == NULL)
	{
		printf("Couldn't set 320x240x16 video mode: %s\n", SDL_GetError());
		exit(1);
	}
	
	printf("Video mode set to 320x240x16\n");
	
	/* SDL_ttf */
	
	if (TTF_Init() < 0)
	{
		printf("Couldn't initialize SDL_ttf: %s\n", TTF_GetError());
		exit(1);
	}
	
	printf("SDL_ttf initialized\n");
	
	/* SDL_mixer */
	
	if(Mix_OpenAudio(22050, AUDIO_S16, 2, 512) < 0)
	{	
		printf("Couldn't initialize SDL_mixer: %s\n", Mix_GetError());
		sound_on = 0;
	}
	
	printf("SDL_mixer initialized\n");
	
	return;
}

void quit_SDL(void)
{
	printf("evöL quit\n");

	if(sound_on)
		Mix_CloseAudio();
		
	TTF_Quit();
	
	SDL_Quit();
	return;
}
