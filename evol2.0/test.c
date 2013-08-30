#include <stdio.h>

#include "evol.h"

int main(void)
{
	evol_init();

	evol_set_mode(evol_windowed,400,300);

	evol_window_mode mode;
	int w,h;

	evol_get_mode(&mode,&w,&h);
	printf("Mode: %d, %dx%d\n", mode, w, h);

	/*
	while (1)
	{
		SDL_Event e;
		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				break;
			}
		}
	}
	*/

	evol_quit();
}