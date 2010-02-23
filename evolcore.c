/*
 * evolcore.c -- evöL
 * Copyright (C) 2008 Noé Falzon
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


/* Globals */

SDL_Surface *gScreen;
void putpixel(SDL_Surface * surface, Uint16 x, Uint16 y, Uint32 color);
int sound_on = 1;
int gl_on = 0;
int curses_on = 0;

typedef struct{
	
	TTF_Font *font;
	int cw,ch;	/* Glyph size */
	int w,h;	/* Terminal size */
	
	int N;		/* w*h */
	
	char *c;	/* Character data */
	SDL_Color *fg,*bg;	/* Text and background colors */
	
	
} CursesContext;

CursesContext CC;

/* init(width,height) */

static int l_init (lua_State *L)
{
	int w,h,full;
	
	w = lua_tonumber(L, 1);
	h = lua_tonumber(L, 2);
	full = lua_toboolean(L, 3);
	gl_on = lua_toboolean(L,4);
	
	printf("evöL init (%dx%d)", w, h);
	if(full)
		printf(", fullscreen");
	if(gl_on)
		printf(", OpenGL mode");
	printf("\n");
	
	/* Init SDL */
		
	if( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0 )
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
        }
	
	/* Set video mode */
        
        if(full)
	{
		full = SDL_FULLSCREEN;
	}
	
        if(gl_on)
	{
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		gl_on = SDL_OPENGL;
	}
	        
        gScreen=SDL_SetVideoMode(w,h,32, SDL_HWSURFACE|SDL_DOUBLEBUF|full|gl_on);
        
        if(gScreen == NULL)
	{
		printf("Couldn't set %dx%d video mode: %s\n", w, h, SDL_GetError());
		exit(1);
        }
	
	/* SDL_ttf */
	
	if (TTF_Init() < 0)
	{
		printf("Couldn't initialize SDL_ttf: %s\n", TTF_GetError());
		exit(1);
        }
	
	/* SDL_mixer */
	
	if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 ) < 0)
	{	
		printf("Couldn't initialize SDL_mixer: %s\n", Mix_GetError());
		sound_on = 0;
		/* exit(1); */
	}
	
	return 0;	/* 0 return values */
}

static int l_quit (lua_State *L)
{
	printf("evöL quit\n");
	
	if(curses_on)
	{
		TTF_CloseFont(CC.font);
		free(CC.c);
		free(CC.fg);
		free(CC.bg);
	}
	
	Mix_CloseAudio();
	TTF_Quit(); 
	
	SDL_Quit();
	return 0;	/* 0 return values */
}


/* Graphic functions */

/* fill(x,y,w,h,r,g,b,a) */

static int l_fill (lua_State *L)
{	
	int r,g,b,a;
	SDL_Rect rect, *ptr = NULL;
	SDL_Surface *surf;
	
	rect.x = lua_tonumber(L, 1);
	rect.y = lua_tonumber(L, 2);
	rect.w = lua_tonumber(L, 3);
	rect.h = lua_tonumber(L, 4);
	r = lua_tonumber(L, 5);
	g = lua_tonumber(L, 6);
	b = lua_tonumber(L, 7);
	a = lua_tonumber(L, 8);
	surf = (SDL_Surface *)(int) lua_touserdata(L, 9);
	
	if(rect.w != 0 && rect.h != 0)
		ptr = &rect;
	
	if(surf == NULL)
		surf = gScreen;
	
	SDL_FillRect(surf, ptr, SDL_MapRGBA(surf->format, r,g,b,a));
	
	return 0;	/* 0 return values */
}

static int l_flip (lua_State *L)
{	
	if(gl_on)
		SDL_GL_SwapBuffers();
	else
		SDL_Flip(gScreen);
	
	return 0;	/* 0 return values */
}



/* Dealing with events: return type of event (1,2,3,4) and parameters */

static int l_input (lua_State *L)
{
	SDL_Event e;

	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_KEYDOWN:
				lua_pushnumber(L, 1);
				lua_pushnumber(L, e.key.keysym.sym);
				return 2;
				break;
			case SDL_KEYUP:
				lua_pushnumber(L, 2);
				lua_pushnumber(L, e.key.keysym.sym);
				return 2;
				break;
			case SDL_MOUSEBUTTONDOWN:
				lua_pushnumber(L, 3);
				lua_pushnumber(L, e.button.x);
				lua_pushnumber(L, e.button.y);
				lua_pushnumber(L, e.button.button);
				return 4;
				break;
			case SDL_MOUSEBUTTONUP:
				lua_pushnumber(L, 4);
				lua_pushnumber(L, e.button.x);
				lua_pushnumber(L, e.button.y);
				lua_pushnumber(L, e.button.button);
				return 4;
				break;
			case SDL_MOUSEMOTION:
				break;
			case SDL_QUIT:
				exit(0);
				break;
			default:
				break;
		}
	}
	
	lua_pushnumber(L, 0);
	return 1;
}

/* all in one function: returns the position of the mouse and if button is down */

static int l_mouse(lua_State * L)
{
	int x,y,button;
	int isDown;
	
	button = lua_tonumber(L, 1);
	
	isDown = ( SDL_GetMouseState(&x, &y) & SDL_BUTTON(button) ) != 0 ;
		
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushboolean(L, isDown);
	
	return 3;
}

static int l_isKeyDown(lua_State *L)
{
	Uint8 * keystate = SDL_GetKeyState(0);
	int key;
	
	key = lua_tonumber(L, 1);
	
	lua_pushboolean(L, keystate[key] == 1);
	return 1;		
}

/* Creating images */

static int l_loadImage(lua_State *L)
{
	const char *path;
	SDL_Surface *tmp, *surf;
	
	path = lua_tostring (L, 1);
	
	tmp = IMG_Load(path);
	if ( tmp == NULL )
	{
		printf("Couldn't load image %s. Exiting.\n", path);
		exit(EXIT_FAILURE);
	}
	
	surf = SDL_DisplayFormatAlpha(tmp);
	SDL_FreeSurface(tmp);
	
	lua_pushlightuserdata(L, surf);
	lua_pushnumber(L, surf->w);
	lua_pushnumber(L, surf->h);
	
	return 3;
}

static int l_makeSurf(lua_State *L)
{
	SDL_Surface *surf, *tmp;
	int w,h;
	
	w = lua_tonumber(L, 1);
	h = lua_tonumber(L, 2);
	
	tmp = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_SRCALPHA,w,h,32,0,0,0,0x000000FF);
	if ( tmp == NULL )
	{
		printf("Couldn't create surface. Exiting.\n");
		exit(EXIT_FAILURE);
	}
	
	surf = SDL_DisplayFormatAlpha(tmp);
	SDL_FreeSurface(tmp);
		
	lua_pushlightuserdata(L, surf);
	lua_pushnumber(L, surf->w);
	lua_pushnumber(L, surf->h);
	
	return 3;
}

static int l_blit(lua_State *L)
{
	SDL_Surface *surf, *dest;
	SDL_Rect rect;
	
	surf = (SDL_Surface *) lua_touserdata(L, 1);
	rect.x = lua_tonumber(L, 2);
	rect.y = lua_tonumber(L, 3);
	dest = (SDL_Surface *) lua_touserdata(L, 4);
	
	if(dest == NULL)
		dest = gScreen;
	
	rect.w = surf->w;
	rect.h = surf->h;
	
	SDL_BlitSurface(surf, NULL, dest, &rect );
	
	return 0;
}

static int l_freeSurf(lua_State *L)
{
	SDL_Surface *surf;
	
	surf = (SDL_Surface *) lua_touserdata(L, 1);
	
	SDL_FreeSurface(surf);
	
	return 0;
}

/* timer must be called at each frame. returns dt and fps. if called with argument, capped fps */

static int l_timer(lua_State *L)
{
	static int frames = 0;
	static int prevTime = 0, currTime = 0;
	static int prevFpsUpdate = 0, fpsUpdateFrequency = 1;
	static float dt, fps;
	static int nextFrame = 0;
	
	int max = lua_tonumber(L,1);

	/* Frames rendered */
	frames++;
	
	/* "Current" time is previous time by now. */
	prevTime = currTime;

	/* Get ticks from SDL */
	currTime = SDL_GetTicks();

	/* Convert to number of seconds */
	dt = (currTime - prevTime)/1000.0f;

	/* Update FPS? */
	if((currTime - prevFpsUpdate)/1000.0f > fpsUpdateFrequency)
	{
		fps = frames/fpsUpdateFrequency;
		prevFpsUpdate = currTime;
		frames = 0;
	}
	
	/* Clipped FPS case */
	if(max > 0)
	{
		if(nextFrame > currTime)
		{
			SDL_Delay(nextFrame - currTime);
		}
	
		nextFrame += 1000/max;
	}
	
	lua_pushnumber(L, dt);
	lua_pushnumber(L, fps);
	lua_pushnumber(L, currTime);
	return 3;
}

static int l_putpix(lua_State *L)
{
	int x,y,r,g,b,a;
	SDL_Surface *surf;
	
	x = lua_tonumber(L, 1);
	y = lua_tonumber(L, 2);
	r = lua_tonumber(L, 3);
	g = lua_tonumber(L, 4);
	b = lua_tonumber(L, 5);
	a = lua_tonumber(L, 6);
	surf = (SDL_Surface *) lua_touserdata(L, 7);
	
	if(surf == NULL)
		surf = gScreen;
	
	SDL_LockSurface(surf);
	putpixel(surf, x, y, SDL_MapRGBA(surf->format, r,g,b,a));
	SDL_UnlockSurface(surf);	
	
	return 0;	/* 0 return values */
}

/* Font stuff */

static int l_loadFont(lua_State *L)
{
	TTF_Font *font = NULL;
	const char *path;
	int size;
	int width = 0;
	
	path = lua_tostring(L, 1);
	size = lua_tonumber(L, 2);
	
	font = TTF_OpenFont(path, size);
	
	if (font == NULL)
	{
		printf("Couldn't load font %s. Exiting.\n", path);
		exit(EXIT_FAILURE);
	}
	
	if(TTF_FontFaceIsFixedWidth(font))
	{
		TTF_GlyphMetrics(font, 'a', NULL, NULL, NULL, NULL, &width);
	}

	
	lua_pushlightuserdata(L, font);
	lua_pushnumber(L, width);
	return 2;
}

static int l_printFont(lua_State *L)
{
	TTF_Font *font;
	SDL_Surface *text, *dest, *surf;
	const char *string;
	SDL_Color color;
	SDL_Rect rect;
	int new;


	font = (TTF_Font *) lua_touserdata(L, 1);
	string = lua_tostring(L, 2);
	color.r = lua_tonumber(L, 3);
	color.g = lua_tonumber(L, 4);
	color.b = lua_tonumber(L, 5);	
	rect.x = lua_tonumber(L, 6);
	rect.y = lua_tonumber(L, 7);
	dest = (SDL_Surface *) lua_touserdata(L, 8);
	new = lua_toboolean(L, 9);
	
	if(dest == NULL)
		dest = gScreen;
	
	text = TTF_RenderUTF8_Blended(font, string, color);
	
	if(!new)
	{
		rect.w = text->w;
		rect.h = text->h;
		
		SDL_BlitSurface(text, NULL, dest, &rect);
		SDL_FreeSurface(text);
		
		return 0;
	}
	else
	{	
		surf = SDL_DisplayFormatAlpha(text);
		SDL_FreeSurface(text);
		
		lua_pushlightuserdata(L, surf);
		lua_pushnumber(L, surf->w);
		lua_pushnumber(L, surf->h);
		
		return 3;
	}
	/* Shouldn't get there */
	return 0;
}

static int l_freeFont(lua_State *L)
{
	TTF_Font *font;
	
	font = (TTF_Font *) lua_touserdata(L, 1);
	TTF_CloseFont(font);
	
	return 0;
}

static int l_showCursor(lua_State *L)
{
	int show;
	
	show = lua_toboolean(L, 1);
	if(show)
		SDL_ShowCursor(SDL_ENABLE);
	else
		SDL_ShowCursor(SDL_DISABLE);
		
	return 0;	
}

/* types: 0 = line, 1 = rect, 2 = circle */
/* aa_fill: 0 = normal, 1 = antialiased, 2 = filled */
static int l_draw (lua_State *L)
{	
	int r,g,b,a;
	int u,v,w,x;
	SDL_Surface *surf;
	int type, aa_fill;
	
	u = lua_tonumber(L, 1);
	v = lua_tonumber(L, 2);
	w = lua_tonumber(L, 3);
	x = lua_tonumber(L, 4);
	r = lua_tonumber(L, 5);
	g = lua_tonumber(L, 6);
	b = lua_tonumber(L, 7);
	a = lua_tonumber(L, 8);
	type = lua_tonumber(L, 9);
	aa_fill = lua_tonumber(L, 10);

	surf = (SDL_Surface *) lua_touserdata(L, 11);	
	
	if(surf == NULL)
		surf = gScreen;
	
	if(type == 0 && aa_fill == 0)
		lineRGBA(surf, u, v, w, x, r, g, b, a);
	else if(type == 0 && aa_fill == 1)
		aalineRGBA(surf, u, v, w, x, r, g, b, a);
	else if(type == 1 && aa_fill == 0)
		rectangleRGBA(surf, u, v, w, x, r, g, b, a);
	else if(type == 1 && aa_fill == 2)
		boxRGBA(surf, u, v, w, x, r, g, b, a);
	else if(type == 2 && aa_fill == 0)
		circleRGBA(surf, u, v, w, r, g, b, a);
	else if(type == 2 && aa_fill == 1)
		aacircleRGBA(surf, u, v, w, r, g, b, a);
	else if(type == 2 && aa_fill == 2)
		filledCircleRGBA(surf, u, v, w, r, g, b, a);
	

	
	return 0;	/* 0 return values */
}

/* aa_fill: 0 = normal, 1 = antialiased, 2 = filled */
static int l_drawTri (lua_State *L)
{	
	int r,g,b,a;
	int u,v,w,x,y,z;
	SDL_Surface *surf;
	int aa_fill;
	
	u = lua_tonumber(L, 1);
	v = lua_tonumber(L, 2);
	w = lua_tonumber(L, 3);
	x = lua_tonumber(L, 4);
	y = lua_tonumber(L, 5);
	z = lua_tonumber(L, 6);
	r = lua_tonumber(L, 7);
	g = lua_tonumber(L, 8);
	b = lua_tonumber(L, 9);
	a = lua_tonumber(L, 10);
	aa_fill = lua_tonumber(L, 11);

	surf = (SDL_Surface *) lua_touserdata(L, 12);	
	
	if(surf == NULL)
		surf = gScreen;
	
	if(aa_fill == 0)
		trigonRGBA(surf, u, v, w, x, y, z, r, g, b, a);	
	else if(aa_fill == 1)
		aatrigonRGBA(surf, u, v, w, x, y, z, r, g, b, a);
	else if(aa_fill == 2)
		filledTrigonRGBA(surf, u, v, w, x, y, z, r, g, b, a);

	return 0;	/* 0 return values */
}

static int l_loadSound (lua_State *L)
{
	Mix_Chunk *sound;
	
	const char *path;
	
	if(!sound_on)
		return 0;
		
	path = lua_tostring(L, 1);
	
	sound = Mix_LoadWAV(path);
	if (sound == NULL)
	{
		printf("Couldn't load sound %s. Exiting.\n", path);
		exit(EXIT_FAILURE);
	}
	
	lua_pushlightuserdata(L, sound);
	return 1;
}

static int l_playSound (lua_State *L)
{
	Mix_Chunk *sound;
	int loop;
	
	if(!sound_on)
		return 0;
	
	sound = (Mix_Chunk *) lua_touserdata(L, 1),
	loop = lua_tonumber(L, 2);
	
	Mix_PlayChannel(-1, sound, loop);
	
	return 0;
}

static int l_freeSound (lua_State *L)
{
	Mix_Chunk *sound;
	
	if(!sound_on)
		return 0;
	
	sound = (Mix_Chunk *) lua_touserdata(L, 1),
	
	Mix_FreeChunk(sound);
	
	return 0;
}

static int l_loadMusic (lua_State *L)
{
	Mix_Music *music;
	const char *path;
	
	if(!sound_on)
		return 0;
	
	path = lua_tostring(L, 1);
	
	music = Mix_LoadMUS(path);
	if (music == NULL)
	{
		printf("Couldn't load music %s. Exiting.\n", path);
		exit(EXIT_FAILURE);
	}
	
	lua_pushlightuserdata(L, music);
	return 1;
}

static int l_playMusic (lua_State *L)
{
	Mix_Music *music;
	int loop;
	
	if(!sound_on)
		return 0;
	
	music = (Mix_Music *) lua_touserdata(L, 1),
	loop = lua_tonumber(L, 2);
	
	Mix_PlayMusic(music, loop);
	
	return 0;
}

static int l_pauseMusic (lua_State *L)
{
	if(!sound_on)
		return 0;
	
	if( Mix_PausedMusic() )
	{
		Mix_ResumeMusic();
	}
	else
	{
		Mix_PauseMusic();
	}
		
	return 0;
}

static int l_musicVolume (lua_State *L)
{
	int vol, retvol;
	
	if(!sound_on)
		return 0;
		
	vol = lua_tonumber(L, 1);
	retvol = Mix_VolumeMusic(vol);
	
	lua_pushnumber(L, retvol);
	return 1;
}

static int l_freeMusic (lua_State *L)
{
	Mix_Music *music;
	
	if(!sound_on)
		return 0;
	
	music = (Mix_Music *) lua_touserdata(L, 1);
	
	
	Mix_FreeMusic(music);
	
	return 0;
}

/* Curses mode */


static int l_curses_init (lua_State *L)
{
	int size;
	const char *font;
	
	CC.w = lua_tonumber(L, 1);
	CC.h = lua_tonumber(L, 2);
	font = lua_tostring(L, 3);
	size = lua_tonumber(L,4);
	
	printf("evöL init (%dx%d), curses mode\n", CC.w, CC.h);
	
	/* Init SDL */
		
	if( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0 )
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
        }
	
	/* SDL_ttf */
	
	if (TTF_Init() < 0)
	{
		printf("Couldn't initialize SDL_ttf: %s\n", TTF_GetError());
		exit(1);
        }
	
	/* SDL_mixer */
	
	if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 ) < 0)
	{	
		printf("Couldn't initialize SDL_mixer: %s\n", Mix_GetError());
		sound_on = 0;
		/* exit(1); */
	}
	
	/* Load font */
	CC.font = TTF_OpenFont(font,size);
	if(!CC.font)
	{
		printf("Couldn't load font: %s\n", TTF_GetError());
		exit(1);
	}

	TTF_GlyphMetrics(CC.font,'@',NULL,NULL,NULL,NULL,&CC.cw);
	CC.ch = TTF_FontLineSkip(CC.font);

	/* Set video mode */
	gScreen=SDL_SetVideoMode(CC.w*CC.cw,CC.h*CC.ch,32, SDL_HWSURFACE|SDL_DOUBLEBUF);
        
        if(gScreen == NULL)
	{
		printf("Couldn't set %dx%d video mode: %s\n", CC.w*CC.cw, CC.h*CC.ch, SDL_GetError());
		exit(1);
	}
	
	/* Init curses context */
	CC.N = CC.w * CC.h;
	
	CC.c = malloc(CC.N * sizeof(char));
	CC.fg = malloc(CC.N * sizeof(SDL_Color));
	CC.bg = malloc(CC.N * sizeof(SDL_Color));
	if(CC.c == NULL || CC.c == NULL || CC.c == NULL)
	{
		printf("Couldn't allocate memory for curses data.\n");
		exit(1);
	}
	
	memset(CC.c, ' ', CC.N);
	memset(CC.fg, 0, CC.N * sizeof(SDL_Color));
	memset(CC.bg, 255, CC.N * sizeof(SDL_Color));
	
	curses_on = 1;
	
	/* Return some font information */
	lua_pushnumber(L, CC.cw);
	lua_pushnumber(L, CC.ch);

	return 2;
}

static int l_curses_refresh (lua_State *L)
{
	int i,j;
	SDL_Rect r;
	SDL_Surface *glyph;
	SDL_Color col;
	char ch[2] = {0,0};
	
	for (i = 0; i < CC.w; i++)
	{
		for (j = 0; j < CC.h; j++)
		{
			r.x = i*CC.cw;
			r.y = j*CC.ch;
			r.w = CC.cw;
			r.h = CC.ch;
			
			ch[0] = CC.c[i + j*CC.w];
						
			glyph = TTF_RenderText_Blended(CC.font, ch, CC.fg[i + j*CC.w]);
			
			/* Background */
			col = CC.bg[i + j*CC.w];
			SDL_FillRect(gScreen, &r, SDL_MapRGB(gScreen->format, col.r, col.g, col.b));
			
			/* Glyph */
			r.x = i*CC.cw;
			r.y = j*CC.ch;
			r.w = CC.cw;
			r.h = CC.ch;
			SDL_BlitSurface(glyph,NULL,gScreen,&r);
			
			SDL_FreeSurface(glyph);
		}	
	}
	
	return 0;
}

static int l_curses_addch (lua_State *L)
{
	int x,y;
	const char *ch;
	char ch2;
	
	ch = lua_tostring(L, 1);
	x = lua_tonumber(L, 2);
	y = lua_tonumber(L, 3);
	
	
	/* Clipping */
	if(x < 0 || x >= CC.w || y < 0 || y >= CC.h)
		return 0;
	
	/* Prevent non printables */
	if(ch[0] <= 32 || (ch[0] >= 0x7f && ch[0] <= 0x9f))
		ch2 = ' ';
	else
		ch2 = ch[0];
	
	CC.c[x + y*CC.w] = ch2;
	
	return 0;
}

static int l_curses_setcol (lua_State *L)
{
	int x,y, r,g,b, br,bg,bb;
	int ind;
	
	x = lua_tonumber(L, 1);
	y = lua_tonumber(L, 2);
	r = luaL_optnumber(L, 3, -1);
	g = luaL_optnumber(L, 4, -1);
	b = luaL_optnumber(L, 5, -1);
	br = luaL_optnumber(L, 6, -1);
	bg = luaL_optnumber(L, 7, -1);
	bb = luaL_optnumber(L, 8, -1);
	
	/* Clipping */
	if(x < 0 || x >= CC.w || y < 0 || y >= CC.h)
		return 0;
	
	ind = x + CC.w*y;
	
	if (r >= 0) CC.fg[ind].r = r;
	if (g >= 0) CC.fg[ind].g = g;
	if (b >= 0) CC.fg[ind].b = b;
	
	if (br >= 0) CC.bg[ind].r = br;
	if (bg >= 0) CC.bg[ind].g = bg;
	if (bb >= 0) CC.bg[ind].b = bb;
	
	return 0;
}

static int l_curses_fill (lua_State *L)
{
	int x,y, w,h, r,g,b, br,bg,bb;
	const char *ch;
	char ch2;
	int ind,i,j;
	
	x = lua_tonumber(L, 1);
	y = lua_tonumber(L, 2);
	w = lua_tonumber(L, 3);
	h = lua_tonumber(L, 4);
	ch = lua_tostring(L, 5);
	r = luaL_optnumber(L, 6, -1);
	g = luaL_optnumber(L, 7, -1);
	b = luaL_optnumber(L, 8, -1);
	br = luaL_optnumber(L, 9, -1);
	bg = luaL_optnumber(L, 10, -1);
	bb = luaL_optnumber(L, 11, -1);
	
	for (i = x; i < x+w ; i++)
	{
		for (j = y; j < y+h ; j++)
		{
			/* Clipping */
			if(i < 0 || i >= CC.w || j < 0 || j >= CC.h)
				continue;

			ind = i + CC.w*j;
			
			
			if(ch)
			{
				/* Prevent non printables */
				if(ch[0] <= 32 || (ch[0] >= 0x7f && ch[0] <= 0x9f))
					ch2 = ' ';
				else
					ch2 = ch[0];
				
				CC.c[ind] = ch2;
			}
			
			if (r >= 0 && g >= 0 && b >= 0)
			{
				CC.fg[ind].r = r;
				CC.fg[ind].g = g;
				CC.fg[ind].b = b;
			}
			
			if (br >= 0 && bg >= 0 && bb >= 0)
			{
				CC.bg[ind].r = br;
				CC.bg[ind].g = bg;
				CC.bg[ind].b = bb;
			}
		}
		
	}
	
	return 0;
}


/* Making this a Lua module */

static const struct luaL_reg R[] = {
	{"init", l_init},
	{"quit", l_quit},
	{"fill", l_fill},
	{"flip", l_flip},
	{"input", l_input},
	{"mouse", l_mouse},
	{"isKeyDown", l_isKeyDown},
	{"loadImage", l_loadImage},
	{"makeSurf", l_makeSurf},
	{"blit", l_blit},
	{"freeSurf", l_freeSurf},
	{"timer", l_timer},
	{"putpix", l_putpix},
	{"loadFont", l_loadFont},
	{"printFont", l_printFont},
	{"freeFont", l_freeFont},
	{"showCursor", l_showCursor},
	{"draw", l_draw},
	{"drawTri", l_drawTri},
	{"loadSound", l_loadSound},
	{"loadMusic", l_loadMusic},
	{"playSound", l_playSound},
	{"playMusic", l_playMusic},
	{"pauseMusic", l_pauseMusic},
	{"musicVolume", l_musicVolume},
	{"freeSound", l_freeSound},
	{"freeMusic", l_freeMusic},
	
	{"curses_init", l_curses_init},
	{"curses_refresh", l_curses_refresh},
	{"curses_addch", l_curses_addch},
	{"curses_setcol", l_curses_setcol},
	{"curses_fill", l_curses_fill},
	
	{NULL, NULL}
};

int luaopen_evolcore (lua_State *L)
{
	luaL_register(L, "evolcore", R);
	return 1;
}



/*********************************************/

void putpixel(SDL_Surface * surface, Uint16 x, Uint16 y, Uint32 color)
{
    /* Nombre de bits par pixels de la surface d'écran */
    Uint8 bpp = surface->format->BytesPerPixel;
    /* Pointeur vers le pixel à remplacer (pitch correspond à la taille 
       d'une ligne d'écran, c'est à dire (longueur * bitsParPixel) 
       pour la plupart des cas) */
    Uint8 * p = ((Uint8 *)surface->pixels) + y * surface->pitch + x * bpp;
     switch(bpp)
    {
	  case 1:
		*p = (Uint8) color;
		break;
        case 2:
            *(Uint16 *)p = (Uint16) color;
            break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                *(Uint16 *)p = ((color >> 8) & 0xff00) | ((color >> 8) & 0xff);
                *(p + 2) = color & 0xff;
            }
            else
            {
                *(Uint16 *)p = color & 0xffff;
                *(p + 2) = ((color >> 16) & 0xff) ;
            }
            break;
        case 4:
            *(Uint32 *)p = color;
            break;
    }
}
