#pragma once

enum board_spaces {EMPTY, PLAYER, START, FINISH, HORIZONTAL, VERTICAL, CORNER_RIGHT, CORNER_LEFT, CORNER_U_RIGHT, CORNER_U_LEFT, CENTER, T_UP, T_RIGHT, T_DOWN, T_LEFT, UNCONNECTED, PVERTICAL, PHORIZONTAL, PCORNER_LEFT, PCORNER_RIGHT, PCORNER_U_RIGHT, PCORNER_U_LEFT};
typedef enum board_spaces board_space;

enum move_directions {UP, LEFT, RIGHT, DOWN, NONE};
typedef enum move_directions move_direction;

board_space find_wall_type(board_space** board, unsigned x, unsigned y, unsigned xmax, unsigned ymax);

unsigned char space_is_wall(board_space space);
