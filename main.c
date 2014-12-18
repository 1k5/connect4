#include <stdio.h>

#include <lua.h>
#include <lauxlib.h>

#include "c4.h"


#define MATCHES		10000


int init_state(lua_State **Lp, int i, char *ai) {
	char *file;
	int ret = 0;

	if ( (*Lp = luaL_newstate()) == NULL ) {
		fprintf(stderr, "Could not create new lua state.\n");
		ret = 1;
	} else {
		luaL_openlibs(*Lp);

		asprintf(&file, "%s.lua", ai);

		printf("Loading ai for player %d: '%s' in state %x.\n",
		       i, file, *Lp);

		if ( luaL_loadfile(*Lp, file) != LUA_OK
		    ||  lua_pcall(*Lp, 0, 0, 0) != LUA_OK )
		{
			fprintf(stderr, "Could not load/run ai file.\n");
			ret = 1;
		};
	};

	return ret;
};


int play_games(lua_State *L[]) {
	int win[3] = { 0, 0, 0 };
	int ret = 0;
	int i;

	printf("Initializing game: ");
	if ( c4_init(L) == 0 ) {
		printf("OK.\n");

		for (i=0; i<MATCHES; i++) {
			win[ c4_play(L) ]++;
			c4_reset();
		};

		printf("Player 1: %d,\tPlayer 2: %d,\tDraw: %d.\n",
		       win[1], win[2], win[0] );

	} else {
		fprintf(stderr, "Error in resource allocation.\n");
		ret = 1;
	};

	return ret;
};


int main(int argc, char **argv) {
	lua_State *L[2] = { NULL, NULL };
	int ret = 0;
	int i;

	if (argc == 3) {

		for (i=0; i<2; i++) {
			if ( (ret = init_state(&L[i], i, argv[i+1])) != 0 ) {
				break;
			};
		};

		if (ret == 0) {
			ret = play_games(L);
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

