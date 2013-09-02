#include <stdio.h>
#include <math.h>

#include "evol.h"

int main(void)
{
	evol_init();

	evol_set_mode(evol_windowed,400,300);

	evol_window_mode mode;
	int w,h;

	evol_get_mode(&mode,&w,&h);
	printf("Mode: %d, %dx%d\n", mode, w, h);
	evol_get_native_size(&w,&h);
	printf("Native: %dx%d\n", w, h);

	evol_show_cursor(0);
	
	evol_image image = evol_load_image("buy.png");
	if(image == NULL)
	{
		printf("No texture :/\n");
	}

	evol_get_image_size(image, &w, &h);
	printf("Image: %dx%d\n", w, h);

	int done = 0;
	while (1)
	{
		double dt,fps,t;
		evol_update_timer(30, &dt, &fps, &t);
		
		evol_clear_screen(220,40,100);

		evol_draw_image(image, 20, 10);

		evol_update_screen();

		if(t > 6.0)
		{
			break;
		}
	}

	evol_free_image(image);
	evol_quit();
}