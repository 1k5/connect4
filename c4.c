#include <stdlib.h>
#include <strings.h>

#include <lua.h>
#include <lauxlib.h>

#include "c4.h"
#include "c4l.h"


#define BOARD(x,y)	( G.board[(x)+(y)*WIDTH] )


static struct c4game G;


/** Initialize Game.
 *
 * Allocate game resources and register global game object 'G' in each lua state.
 *
 * \param L	The players' lua states.
 * \return	Status code: -1 in case of an error, 0 in case of success.
 */
int c4_init(lua_State *L[]) {
	struct c4game **g[2] = { NULL, NULL };
	int ret = 0;
	int i, j;

	G.width = WIDTH;
	G.height = HEIGHT;

	if ( (G.board = calloc(WIDTH*HEIGHT, sizeof(int))) == NULL ) {
		ret = -1;
	} else if ( (G.prev_move = calloc(2, sizeof(int))) == NULL ) {
		free(G.board);
		ret = -1;
	} else {
		for (i=0; i<2; i++) {
			if ( (g[i] = lua_newuserdata(L[i], sizeof(*g))) == NULL  ||  !c4l_register(L[i]) ) {
				free(G.board);
				free(G.prev_move);
				ret = -1;
				break;
			} else {
				*g[i] = &G;
			};
		};
	};

	return ret;
};


static int c4_makemove(int move, int p) {
	int y;
	int ret = 1;

	if (move < 0  ||  move >= WIDTH) {
		printf("Invalid move: out of bounds.\n");
		ret = 0;
	} else if (BOARD(move, HEIGHT-1) != 0) {
		printf("Invalid move: column is full.\n");
		ret = 0;
	} else {
		/* place stone from top */
		for (y=0; y<HEIGHT; y++) {
			if (!BOARD(move, y)) {
				BOARD(move, y) = p+1;
				break;
			};
		};
	};

	return ret;
};


/*** Given move wins the game?
 *
 * Find the top stone in a given column, then count for each of the four
 * directions (horizontal, vertical, diagonals) the number of stones in a row
 * connected to the given stone and of the same player/color.
 *
 * TODO: Clean up this terrible mess!
 *
 * \param move	The column in which the potential winning stone is.
 * \return	True (1) in case move was a winning move, false (0) otherwise.
 */
static int c4_movewins(int move) {
	int ret = 0;
	int x0, y0;
	int dx, dy;
	int x, y;
	int i, c;

	x0 = move;
	for (y0=0; BOARD(x0,y0)!=0; y0++) {};
	y0--; /* found first empty slot. */

	dy = 1;
	for (dx=-1; dx<=1; dx++) {
		c = 1;
		for (i=1; i<=3; i++) {
			x = x0 + i*dx;
			y = y0 + i*dy;
			if ( 0<=x && x<WIDTH  &&  0<=y && y<HEIGHT
			     &&  BOARD(x,y) == BOARD(x0,y0) )
			{
				c++;
			} else {
				break;
			};
		};
		for (i=1; i<=3; i++) {
			x = x0 - i*dx;
			y = y0 - i*dy;
			if ( 0<=x && x<WIDTH  &&  0<=y && y<HEIGHT
			     &&  BOARD(x,y) == BOARD(x0,y0) )
			{
				c++;
			} else {
				break;
			};
		};
		if (c >= 4) {
			ret = 1;
			break;
		};
	};
	
	if (ret == 0) {
		dy = 0; dx = 1;
		c = 1;
		for (i=1; i<=3; i++) {
			x = x0 + i*dx;
			y = y0 + i*dy;
			if ( 0<=x && x<WIDTH  &&  0<=y && y<HEIGHT
			     &&  BOARD(x,y) == BOARD(x0,y0) )
			{
				c++;
			} else {
				break;
			};
		};
		for (i=1; i<=3; i++) {
			x = x0 - i*dx;
			y = y0 - i*dy;
			if ( 0<=x && x<WIDTH  &&  0<=y && y<HEIGHT
			     &&  BOARD(x,y) == BOARD(x0,y0) )
			{
				c++;
			} else {
				break;
			};
		};
		if (c >= 4) {
			ret = 1;
		};
	};

	return ret;
};


/** Play a game.
 *
 * Play a single game of connect four.  A player making an error
 * automatically loses.  An unattributed error leads to a draw.
 *
 * \return	Status code: 0 in case of a draw, or the winning player's
 *		number (1 or 2.)
 */
int c4_play(lua_State *L[]) {
	int p, move;
	int i;
	int ret = -1;

	printf("Playing a game.\n");
	for (i=0; i<WIDTH*HEIGHT; i++) {
		p = G.current_player = i % 2;
		lua_getglobal(L[p], "move");
		lua_call(L[p], 0, 1);
		move = lua_tointeger(L[p], -1) - 1;
		lua_pop(L[p], 1);

		if (c4_makemove(move, p)) {
			if (c4_movewins(move)) {
				ret = p;
				printf("Player %d played a winning move!\n", p+1);
				break;
			};
		} else {
			ret = (p+1)%2;
			printf("Player %d lost by an invalid move!\n", p+1);
			break;
		};
	};

	return ret+1;
};


/** Reset game.
 *
 * Reset game resources for next game.
 */
void c4_reset(void) {
	bzero(G.board, (WIDTH*HEIGHT) * sizeof(*G.board));
	bzero(G.prev_move, (2) * sizeof(*G.prev_move));
};
