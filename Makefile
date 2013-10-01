LUA_DIR=lua

PLAT= 
EVOL_LIBS= 

SRC= evol.c evol_lua.c main.c
CFLAGS= -I $(LUA_DIR)/src

PLATFORMS= macosx linux

default:
	@echo "Please choose platform:" $(PLATFORMS)

macosx:
	make evol EVOL_LIBS="-framework SDL2 -framework SDL2_image -framework SDL2_ttf" PLAT=macosx

linux:
	make evol EVOL_LIBS="-lSDL2 -lSDL2_image -lSDL2_ttf" PLAT=linux

$(LUA_DIR)/src/liblua.a:
	cd $(LUA_DIR) && make $(PLAT)

evol: $(SRC) $(LUA_DIR)/src/liblua.a
	gcc -o evol $(SRC) $(LUA_DIR)/src/liblua.a $(EVOL_LIBS)

clean:
	cd $(LUA_DIR) && make clean
	rm evol