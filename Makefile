LUA_DIR=lua

main: main.c evol.o lua_evol.o $(LUA_DIR)/src/liblua.a
	gcc -o main main.c evol.o lua_evol.o $(LUA_DIR)/src/liblua.a -framework SDL2 -framework SDL2_image -framework SDL2_ttf

lua/src/liblua.a:
	cd $(LUA_DIR) && make macosx

evol.o: evol.c
	gcc -o evol.o -c evol.c

lua_evol.o: evol_lua.c
	gcc -o lua_evol.o -c evol_lua.c -I $(LUA_DIR)/src