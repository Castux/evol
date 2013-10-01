LUA_DIR=lua

PLAT= 
EVOL_LIBS= 

SRC= evol.c evol_lua.c main.c
CFLAGS= $(shell sdl2-config --cflags) -I $(LUA_DIR)/src

PLATFORMS= macosx linux

default:
	@echo "Please choose platform:" $(PLATFORMS)

macosx:
	make evol EVOL_LIBS="-framework SDL2 -framework SDL2_image -framework SDL2_ttf" PLAT=macosx

linux:
	make evol EVOL_LIBS="$(shell sdl2-config --libs) -lSDL2 -lSDL2_image -lSDL2_ttf -lm -ldl" PLAT=linux

$(LUA_DIR)/src/liblua.a:
	cd $(LUA_DIR) && make $(PLAT)

evol: $(SRC) $(LUA_DIR)/src/liblua.a
	gcc -o evol $(CFLAGS) $(SRC) $(LUA_DIR)/src/liblua.a $(EVOL_LIBS)

clean:
	cd $(LUA_DIR) && make clean
	rm evol