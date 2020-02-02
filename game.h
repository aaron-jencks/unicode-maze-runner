#pragma once

#include <stddef.h>
#include <time.h>

#include "player.h"
#include "gamedefs.h"

typedef struct _game_t {
	player p;
	board_space** board;
	size_t board_width;
	size_t board_height;
	size_t turn;
	size_t round;
	time_t start_time;
} game_t;

typedef game_t* game;


game create_game(player p);

void destroy_game(game g);

game game_reset(game g);

board_space** generate_maze(unsigned width, unsigned height);

unsigned* find_player(game g);

unsigned char game_is_win(game g);

game take_turn(game g, move_direction direction);

game score_round(game g);
