typedef enum
{
	evol_windowed,
	evol_fullscreen
} evol_window_mode;

typedef unsigned char byte;

/* System and video */

void evol_init();
void evol_quit();
void evol_get_native_size(int *w, int *h);
void evol_set_mode(evol_window_mode mode, int width, int height);
void evol_get_mode(evol_window_mode *mode, int *width, int *height);
void evol_update_timer(int fps_cap, double *dt, double *fps, double *t);
void evol_show_cursor(int toggle);
void evol_clear_screen(byte r, byte g, byte b);
void evol_update_screen();

/* Graphics */

typedef void* evol_image;

evol_image evol_load_image(const char *path);
void evol_free_image(evol_image image);
void evol_get_image_size(evol_image image, int *width, int *height);
void evol_set_image_modulation(evol_image image, byte r, byte g, byte b, byte a);
void evol_draw_image(evol_image image, int x, int y);
void evol_draw_image2(evol_image image, int x, int y, double scale_x, double scale_y, int pivot_x, int pivot_y, double angle);

/* Events */

typedef enum
{
	evol_mouse = 0,
	evol_keyboard,
	evol_gamepad,
	evol_system
} evol_device;

typedef enum
{
	evol_button_event = 0,
	evol_axis_event,
	evol_quit_event
} evol_event_type;

typedef enum
{
	evol_up = 0,
	evol_down
} evol_button_state;

typedef enum
{
	evol_mouse_motion = 0,
	evol_mouse_wheel
} evol_mouse_axis;

typedef enum
{
	evol_left_stick = 0,
	evol_right_stick,
	evol_triggers
} evol_gamepad_axis;

typedef struct
{
	evol_device			device;
	evol_event_type		type;
	int					device_num;
	int					elem_num;
	evol_button_state	state;		// used for buttons
	int					x;			// used for axes
	int					y;
} evol_event;

int evol_input(evol_event *event);
const char* evol_get_key_name(int key);

/* Text */

typedef void* evol_font;

evol_font evol_load_font(const char *path, int size);
void evol_free_font(evol_font font);
evol_image evol_draw_font(evol_font font, const char *text, byte r, byte g, byte b);