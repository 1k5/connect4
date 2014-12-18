#include <stdio.h>

#include <lua.h>
#include <lauxlib.h>

#include "c4.h"


#define MATCHES		10000


int main(int argc, char **argv) {
	lua_State *L[2] = { NULL, NULL };
	char *file[2] = { NULL, NULL };
	int ret = 0;
	int win[3] = { 0, 0, 0 };
	int i;

	if (argc == 3) {

		for (i=0; i<2; i++) {
			if ( (L[i] = luaL_newstate()) == NULL ) {
				fprintf(stderr, "Could not create new lua state.\n");
				ret = 1;
				break;
			} else {
				luaL_openlibs(L[i]);
        
				asprintf(&file[i], "%s.lua", argv[1+i]);
        
				printf("Loading ai for player %d: '%s' in state %x.\n", i, file[i], L[i]);
        
				if ( luaL_loadfile(L[i], file[i]) != LUA_OK  ||  lua_pcall(L[i], 0, 0, 0) != LUA_OK ) {
					fprintf(stderr, "Could not load/run ai file.\n");
					ret = 1;
					break;
				};
			};
		};

		if (ret == 0) {

			printf("Initializing game: ");
			if ( c4_init(L) == 0 ) {
				printf("OK.\n");

				for (i=0; i<MATCHES; i++) {
					win[ c4_play(L) ]++;
					c4_reset();
				};

				printf("Player 1: %d,\tPlayer 2: %d,\tDraw: %d.\n", win[1], win[2], win[0]);

			} else {

				fprintf(stderr, "Error in resource allocation.\n");
				ret = 1;

			};

		};

		for (i=0; i<2; i++) {
			if ( L[i] != NULL ) {
				lua_close(L[i]);
			};
		};

	} else {

		fprintf(stderr, "Wrong number of arguments.\n");
		ret = 1;

	};

	return ret;
};

