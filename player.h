#pragma once

#include <stddef.h>

#include "gamedefs.h"


typedef struct _player_t {
	char* name;
	size_t score;
	move_direction previous_move;
} player_t;

typedef player_t* player;


player create_player(const char* name);

void destroy_player(player p);
