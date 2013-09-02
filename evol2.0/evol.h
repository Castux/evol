typedef enum
{
	evol_windowed,
	evol_fullscreen
} evol_window_mode;

/* System and video */

void evol_init();
void evol_quit();
void evol_get_native_size(int *w, int *h);
void evol_set_mode(evol_window_mode mode, int width, int height);
void evol_get_mode(evol_window_mode *mode, int *width, int *height);
void evol_update_timer(int fps_cap, double *dt, double *fps, double *t);
void evol_show_cursor(int toggle);
void evol_clear_screen(unsigned char r, unsigned char g, unsigned char b);
void evol_update_screen();

/* Graphics */

typedef void* evol_image;

evol_image evol_load_image(const char *path);
void evol_free_image(evol_image image);
void evol_get_image_size(evol_image image, int *width, int *height);
void evol_draw_image(evol_image image, int x, int y);