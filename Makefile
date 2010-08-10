evolcore.dll: evolcore.c
	i586-mingw32msvc-gcc evolcore.c -shared -o evolcore.dll -Wall -I/usr/i586-mingw32msvc/include -I/usr/i586-mingw32msvc/include/SDL -lSDL_image -lSDL_ttf -lSDL_gfx -lSDL_mixer -lSDL -llua5.1 -Wl,--exclude-symbols,SDL_gfx_NULL_THUNK_DATA,--exclude-symbols,SDL_image_NULL_THUNK_DATA,--exclude-symbols,SDL_mixer_NULL_THUNK_DATA,--exclude-symbols,SDL_ttf_NULL_THUNK_DATA

evol_dingux: evolcore.c evol_dingux.c
	mipsel-linux-gcc evolcore.c evol_dingux.c -o evol_dingux -Wall -I/opt/mipsel-linux-uclibc/usr/include -I/opt/mipsel-linux-uclibc/usr/include/SDL -lSDL_image -lSDL_ttf -lSDL_gfx -lSDL_mixer -lSDL -llua
