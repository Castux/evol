evol.o: evol.c
	gcc -o evol.o -c evol.c

evol.a: evol.o
	ar rcs evol.a evol.o

lua_evol.o: evol.a evol_lua.c
	gcc -fPIC -o lua_evol.o -c evol_lua.c -I ./lua/src

evol.so: lua_evol.o evol.a 
	gcc -shared -o evol.so lua_evol.o evol.a lua/src/liblua.a -framework SDL2 -framework SDL2_image