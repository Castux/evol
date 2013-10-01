#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
	#include "SDL2/SDL.h"
	#include "SDL2_image/SDL_image.h"
	#include "SDL2_ttf/SDL_ttf.h"
#else
	#include "SDL.h"
	#include "SDL_image.h"
	#include "SDL_ttf.h"
#endif

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

	/* Open gamepads */

	int i;
	for(i = 0 ; i < SDL_NumJoysticks() ; i++)
	{
		if(SDL_IsGameController(i))
		{
			SDL_GameControllerOpen(i);
		}
	}

	/* Text */

	TTF_Init();
}

void evol_quit()
{
	SDL_DestroyRenderer(evol_renderer);
	SDL_DestroyWindow(evol_window);

	TTF_Quit();
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

void evol_clear_screen(byte r, byte g, byte b)
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

	if(surface == NULL)
	{
		evol_error("could not load image");
	}

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

void evol_set_image_modulation(evol_image image, byte r, byte g, byte b, byte a)
{
	SDL_SetTextureColorMod((SDL_Texture *) image, r, g, b);
	SDL_SetTextureAlphaMod((SDL_Texture *) image, a);
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

int evol_input(evol_event *event)
{
	SDL_Event e;
	SDL_PollEvent(&e);
	
	switch(e.type)
	{
		/* keyboard */

		case SDL_KEYDOWN:
			
			event->type = evol_button_event;
			event->device = evol_keyboard;
			event->elem_num = e.key.keysym.sym;
			event->state = evol_down;

			return 1;

		case SDL_KEYUP:
			
			event->type = evol_button_event;
			event->device = evol_keyboard;
			event->elem_num = e.key.keysym.sym;
			event->state = evol_up;
			
			return 1;

		/* mouse */

		case SDL_MOUSEBUTTONDOWN:

			event->type = evol_button_event;
			event->device = evol_mouse;
			event->elem_num = e.button.button;
			event->state = evol_down;

			return 1;

		case SDL_MOUSEBUTTONUP:

			event->type = evol_button_event;
			event->device = evol_mouse;
			event->elem_num = e.button.button;
			event->state = evol_up;
			
			return 1;

		case SDL_MOUSEMOTION:
			
			event->type = evol_axis_event;
			event->device = evol_mouse;
			event->elem_num = evol_mouse_motion;
			event->x = e.motion.x;
			event->y = e.motion.y;
			
			return 1;

		case SDL_MOUSEWHEEL:

			event->type = evol_axis_event;
			event->device = evol_mouse;
			event->elem_num = evol_mouse_wheel;
			event->x = e.wheel.x;
			event->y = e.wheel.y;

			return 1;

		/* gamepad */

		case SDL_CONTROLLERBUTTONDOWN:
			
			event->type = evol_button_event;
			event->device = evol_gamepad;
			event->device_num = e.cbutton.which;
			event->elem_num = e.cbutton.button;
			event->state = evol_down;

			return 1;

		case SDL_CONTROLLERBUTTONUP:
			
			event->type = evol_button_event;
			event->device = evol_gamepad;
			event->device_num = e.cbutton.which;
			event->elem_num = e.cbutton.button;
			event->state = evol_up;

			return 1;

		case SDL_CONTROLLERAXISMOTION:
			
			event->type = evol_axis_event;
			event->device = evol_gamepad;
			event->device_num = e.caxis.which;

			switch(e.caxis.axis)
			{
				case SDL_CONTROLLER_AXIS_LEFTX:
					event->elem_num = evol_left_stick;
					event->x = e.caxis.value;
					event->y = 0;
					break;
				case SDL_CONTROLLER_AXIS_LEFTY:
					event->elem_num = evol_left_stick;
					event->x = 0;
					event->y = e.caxis.value;
					break;
				case SDL_CONTROLLER_AXIS_RIGHTX:
					event->elem_num = evol_right_stick;
					event->x = e.caxis.value;
					event->y = 0;
					break;
				case SDL_CONTROLLER_AXIS_RIGHTY:
					event->elem_num = evol_right_stick;
					event->x = 0;
					event->y = e.caxis.value;
					break;
				case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
					event->elem_num = evol_triggers;
					event->x = e.caxis.value;
					event->y = 0;
					break;
				case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
					event->elem_num = evol_triggers;
					event->x = 0;
					event->y = e.caxis.value;
					break;
			}
			
			return 1;

		/* system */

		case SDL_QUIT:

			event->device = evol_system;
			event->type = evol_quit_event;

			return 1;

		/* controller */

		case SDL_CONTROLLERDEVICEADDED:

			SDL_GameControllerOpen(e.cdevice.which);
			return 0;

		default:

			return 0;
	}

	return 0;
}

const char* evol_get_key_name(int key)
{
	return SDL_GetKeyName(key);
}

evol_font evol_load_font(const char *path, int size)
{
	TTF_Font *font = TTF_OpenFont(path, size);

	if(font == NULL)
	{
		evol_error("could not load font");
	}

	return (evol_font) font;
}

void evol_free_font(evol_font font)
{
	TTF_CloseFont((TTF_Font *) font);
}

evol_image evol_draw_font(evol_font font, const char *text, byte r, byte g, byte b)
{
	SDL_Color color = {r, g, b, 255};

	SDL_Surface *surface = TTF_RenderUTF8_Blended((TTF_Font *) font, text, color);
	
	SDL_Texture *texture = SDL_CreateTextureFromSurface(evol_renderer, surface);
	SDL_FreeSurface(surface);

	return (evol_image) texture;
}