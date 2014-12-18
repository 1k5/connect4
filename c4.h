#define WIDTH			7
#define HEIGHT			6


struct c4game {
	int current_player;
	int width, height;
	int *board;
	int *prev_move;
};


int	c4_init(lua_State *L[]);
int	c4_play(lua_State *L[]);
void	c4_reset(void);
