main: main.c evol.o lua_evol.o
	gcc -o main main.c evol.o lua_evol.o lua/src/liblua.a -framework SDL2 -framework SDL2_image -framework SDL2_ttf

evol.o: evol.c
	gcc -o evol.o -c evol.c

lua_evol.o: evol.a evol_lua.c
	gcc -o lua_evol.o -c evol_lua.c -I ./lua/src