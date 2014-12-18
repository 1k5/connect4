#include <lua.h>
#include <lauxlib.h>

#include "c4.h"


#define C4_METATABLE		"C4_MT"


static int c4l_getplayer(lua_State *L) {
	struct c4game **g = luaL_checkudata(L, 1, C4_METATABLE);

	lua_pushnumber(L, (*g)->current_player + 1);

	return 1;
};

static int c4l_getwidth(lua_State *L) {
	struct c4game **g = luaL_checkudata(L, 1, C4_METATABLE);

	lua_pushnumber(L, (*g)->width);

	return 1;
};

static int c4l_getheight(lua_State *L) {
	struct c4game **g = luaL_checkudata(L, 1, C4_METATABLE);

	lua_pushnumber(L, (*g)->height);

	return 1;
};

static int c4l_index(lua_State *L) {
	struct c4game **g = luaL_checkudata(L, 1, C4_METATABLE);
	int x, y;

	luaL_getmetatable(L, C4_METATABLE);
	lua_pushvalue(L, 2);
	lua_rawget(L, -2);

	if ( lua_isnil(L, -1) ) {
		/* no method found */
		y = luaL_checkint(L, 2) - 1;
		luaL_argcheck(L, 0<=y && y<HEIGHT, 2, "index out of range");

		lua_newtable(L);

		for (x=0; x<WIDTH; x++) {
			lua_pushnumber(L, x+1);
			lua_pushnumber(L, (*g)->board[x+y*WIDTH]);
			lua_rawset(L, -3);
		};
	};

	return 1;
};


static const struct luaL_Reg c4l_methods[] = {
	{ "player",	c4l_getplayer	},
	{ "width",	c4l_getwidth	},
	{ "height",	c4l_getheight	},
	{ "__index",	c4l_index	},
	{ NULL,		NULL		}
};


int c4l_register(lua_State *L) {
	int ret = 1;

	if ( luaL_newmetatable(L, C4_METATABLE) == 0 ) {
		ret = 0;
	} else {
		luaL_setfuncs(L, c4l_methods, 0);
		lua_setmetatable(L, -2);
        
		lua_setglobal(L, "C4");
	};

	return ret;
};
