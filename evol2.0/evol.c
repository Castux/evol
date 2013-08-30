#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"

#include "evol.h"

static void evol_error(const char *msg)
{
	printf("evol error: %s. %s\n", msg, SDL_GetError());
	exit(EXIT_FAILURE);
}

static SDL_Window *evol_window;
static SDL_Renderer *evol_renderer;

void evol_init()
{
	int err;

	/* Init SDL */

	err = SDL_Init(SDL_INIT_EVERYTHING);
	if(err != 0)
	{
		evol_error("could not initialize SDL");
	}

	/* Create default window (800x600 windowed) and renderer */

	err = SDL_CreateWindowAndRenderer(800, 600, 0, &evol_window, &evol_renderer);
	if(err != 0)
	{
		evol_error("could not create window");
	}
}

void evol_quit()
{
	SDL_Quit();
}

void evol_get_native_size(int *w, int *h)
{
	int display = SDL_GetWindowDisplayIndex(evol_window);
	if(display < 0)
	{
		evol_error("could not get native size");
	}

	SDL_DisplayMode mode;
	int err = SDL_GetDesktopDisplayMode(display, &mode);
	if(err != 0)
	{
		evol_error("could not get native size");
	}

	*w = mode.w;
	*h = mode.h;
}

void evol_set_mode(evol_window_mode mode, int width, int height)
{
	int err = SDL_SetWindowFullscreen(evol_window, mode == evol_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	if(err != 0)
	{
		evol_error("could not switch to fullscreen");
	}

	if(mode == evol_fullscreen)
	{
		if(width > 0 && height > 0)
		{
			SDL_RenderSetLogicalSize(evol_renderer, width, height);
		}
	}
	else
	{
		SDL_SetWindowSize(evol_window, width, height);
	}
}

void evol_get_mode(evol_window_mode *mode, int *width, int *height)
{
	*mode = (SDL_GetWindowFlags(evol_window) & SDL_WINDOW_FULLSCREEN_DESKTOP) ? evol_fullscreen : evol_windowed;

	int logical_w, logical_h;
	SDL_RenderGetLogicalSize(evol_renderer, &logical_w, &logical_h);

	if(*mode == evol_fullscreen && logical_w > 0 && logical_h > 0)
	{
		*width = logical_w;
		*height = logical_h;
	}
	else
	{
		SDL_GetWindowSize(evol_window, width, height);
	}
}

void evol_update_timer(int fps_cap, double *dt, double *fps, double *t)
{
	static int frames = 0;
	static double last_time = 0.0;
	static double last_fps = 0.0;
	static double last_fps_update = 0.0;
	
	frames++;
	
	float now = SDL_GetTicks() / 1000.0;
	float delta = now - last_time;

	if(fps_cap && delta < 1.0 / fps_cap)
	{
		SDL_Delay( (1.0 / fps_cap - delta) * 1000);
	}

	now = SDL_GetTicks() / 1000.0;

	if(now - last_fps_update > 1.0)
	{
		last_fps = frames / (now - last_fps_update);
		last_fps_update = now;
		frames = 0;
	}

	*dt = now - last_time;
	*fps = last_fps;
	*t = now;

	last_time = now;	
}
