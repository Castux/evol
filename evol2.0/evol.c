#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"

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

	/* Init window and renderer */

	evol_window = NULL;
	evol_renderer = NULL;

	/* SDL_image */

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
}

void evol_quit()
{
	SDL_DestroyRenderer(evol_renderer);
	SDL_DestroyWindow(evol_window);

	IMG_Quit();
	SDL_Quit();
}

void evol_get_native_size(int *w, int *h)
{
	int display = evol_window ? SDL_GetWindowDisplayIndex(evol_window) : 0;

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
	int err;

	if(!evol_window)
	{
		err = SDL_CreateWindowAndRenderer(width, height,
			mode == evol_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0,
			&evol_window, &evol_renderer);

		if(err != 0)
		{
			evol_error("could not create window");
		}
	}
	else
	{
		err = SDL_SetWindowFullscreen(evol_window, mode == evol_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
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
}

void evol_get_mode(evol_window_mode *mode, int *width, int *height)
{
	if(!evol_window)
	{
		*width = 0;
		*height = 0;
	}

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

void evol_show_cursor(int toggle)
{
	SDL_ShowCursor(toggle == 0 ? SDL_DISABLE : SDL_ENABLE);
}

void evol_clear_screen(unsigned char r, unsigned char g, unsigned char b)
{
	SDL_SetRenderDrawColor(evol_renderer, r, g, b, 255);
	SDL_RenderClear(evol_renderer);
}

void evol_update_screen()
{
	SDL_RenderPresent(evol_renderer);
}

/* Graphics */

evol_image evol_load_image(const char *path)
{
	SDL_Surface *surface = IMG_Load(path);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(evol_renderer, surface);
	SDL_FreeSurface(surface);

	return (evol_image) texture;
}

void evol_free_image(evol_image image)
{
	SDL_DestroyTexture((SDL_Texture *) image);
}

void evol_get_image_size(evol_image image, int *width, int *height)
{
	SDL_QueryTexture((SDL_Texture *) image, NULL, NULL, width, height);
}

void evol_draw_image(evol_image image, int x, int y)
{
	SDL_Texture *texture = (SDL_Texture *) image;

	SDL_Rect dst_rect;
	dst_rect.x = x;
	dst_rect.y = y;

	SDL_QueryTexture(texture, NULL, NULL, &dst_rect.w, &dst_rect.h);

	SDL_RenderCopy(evol_renderer, texture, NULL, &dst_rect);
}

void evol_draw_image2(evol_image image, int x, int y, double scale_x, double scale_y, int pivot_x, int pivot_y, double angle)
{
	SDL_Texture *texture = (SDL_Texture *) image;

	SDL_Point center;
	center.x = pivot_x * scale_x;
	center.y = pivot_y * scale_y;

	SDL_Rect dst_rect;
	dst_rect.x = x - center.x;
	dst_rect.y = y - center.y;

	SDL_QueryTexture(texture, NULL, NULL, &dst_rect.w, &dst_rect.h);

	dst_rect.w *= scale_x;
	dst_rect.h *= scale_y;

	SDL_RenderCopyEx(evol_renderer, texture, NULL, &dst_rect, angle, &center, 0);
}