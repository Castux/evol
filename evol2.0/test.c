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

	evol_show_cursor(1);
	
	evol_image image = evol_load_image("tile.png");
	evol_set_image_modulation(image, 255, 255, 255, 128);
	if(image == NULL)
	{
		printf("No texture :/\n");
	}

	evol_image bg = evol_load_image("buy.png");

	evol_get_image_size(image, &w, &h);
	printf("Image: %dx%d\n", w, h);

	while (1)
	{
		double dt,fps,t;
		evol_update_timer(60, &dt, &fps, &t);
		
		evol_event e;
		if(evol_input(&e))
		{
			printf("Event: %d, %d, %d, %d, %d, %d, %d\n", e.device, e.device, e.device_num,
				e.elem_num, e.state, e.x, e.y);

			if(e.device == evol_keyboard)
			{
				printf("Key: %s\n", evol_get_key_name(e.elem_num));
			}

			if(e.type == evol_quit_event)
			{
				break;
			}
		}

		evol_clear_screen(255,255,255);

		evol_draw_image(bg, 30, 30);

		double scale = (cos(t / 10 * 3.14 * 2) / 2 + 0.5) * 2;
		//double scale = 0.7;
		int i,j;
		for(i = 0 ; i < 10 ; i++)
		{
			for(j = 0 ; j < 10 ; j++)
			{
				int this = floor(i * w * scale);
				int next = floor((i+1) * w * scale);
				float targw = next - this;

				this = floor(j * w * scale);
				next = floor((j+1) * w * scale);
				float targh = next - this;

				evol_set_image_modulation(image, i * 255.0 / 10, j * 255.0 / 10, 255, 128);
				evol_draw_image2(image, i * w * scale, j * h * scale, targw/w, targh/h, 0, 0, 0);
			}
		}

		//evol_update_screen();
	}

	evol_free_image(image);
	evol_free_image(bg);
	evol_quit();
}