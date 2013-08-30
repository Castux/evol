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

	
	while (1)
	{
		double dt,fps,t;
		evol_update_timer(60, &dt, &fps, &t);
		printf("%.10f,%.10f,%.10f\n",dt,fps,t);

		if(t > 10.0)
		{
			break;
		}

		float f;
		int i;
		for(i = 0 ; i < 1000000 ; i++)
		{
			f = sqrt(i*200.0);
		}
	}

	evol_quit();
}