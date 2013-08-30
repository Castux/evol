typedef enum
{
	evol_windowed,
	evol_fullscreen
} evol_window_mode;

void evol_init();
void evol_quit();
void evol_get_native_size(int *w, int *h);
void evol_set_mode(evol_window_mode mode, int width, int height);
void evol_get_mode(evol_window_mode *mode, int *width, int *height);