#include <stdlib.h>
#include <time.h>
#include <err.h>

#include "game.h"
#include "gamedefs.h"
#include "player.h"
#include "display.h"
#include "rand_util.h"

#ifdef debug
	#include "stringbuilder.h"
	#include <stdio.h>
#endif


game create_game(player p)
{
	if(p)
	{
		game g = calloc(1, sizeof(game_t));
		if(!g) exit(1);

		g->p = create_player(p->name);
		g->p->score = (p->score) ? p->score : 0;

		g = game_reset(g);

		return g;
	}

	return 0;
}

void destroy_game(game g)
{
	if(g)
	{
		if(g->p) destroy_player(g->p);
		free(g);
	}
}

game game_reset(game g)
{
	if(g)
	{
		// Delete old board
		if(g->board)
		{
			for(int row = 0; row < g->board_height; row++) free(g->board[row]);	// Free each row;
			free(g->board);
		}

		// Update the terminal size
		size_t* term_size = get_screen_size();

		g->board_height = term_size[1];
		if((g->board_height - 1) % 2) g->board_height--;

		g->board_width = term_size[0];
		if((g->board_width - 1) % 2) g->board_width--;

		free(term_size);

		// Resize the board_size
		g->board = generate_maze(g->board_width, g->board_height);

		g->turn = 0;	// Reset the move count;
		time(&g->start_time);
	}

	g->board[1][1] = PLAYER;	// Starts the game off

	return g;
}

board_space** generate_maze(unsigned width, unsigned height)
{
	// Resize the board_size
	board_space** board = malloc(sizeof(board_space*) * height);
	if(!board) err(1, "Memory error");

	for(int row = 0; row < height; row++)
	{
		board[row] = malloc(sizeof(board_space) * width);
		if(!board[row]) err(1, "Memory error");
	}

	#ifdef debug
		stringbuilder dboard = create_stringbuilder();
	#endif

	// Reset the board
	for(int row = 0; row < height; row++)
		for(int col = 0; col < width; col++)
			board[row][col] = EMPTY;

	// Generate the pylons
	for(int row = 0; row < height; row++)
	{

		#ifdef debug
			sb_append_ch(dboard, "[");
		#endif

		for(int col = 0; col < width; col++)
		{
			if(!row || row == height - 1)
			{
				if(!col)
					board[row][col] = (!row) ? CORNER_RIGHT : CORNER_U_RIGHT;
				else if(col == width - 1)
					board[row][col] = (!row) ? CORNER_LEFT : CORNER_U_LEFT;
				else
					board[row][col] = HORIZONTAL;
			}
			else if(!col || col == width - 1) board[row][col] = VERTICAL;
			else if(!(row % 2) && !(col % 2))
			{
				// This is done later, no reason to set it to CENTER twice
				// board[row][col] = CENTER;
				int side = randint(0, 3);	// Decide which side to build the wall on
				switch(side)
				{
					case 0:
						//if(row == 2) board[0][col] = T_DOWN;
						board[row - 1][col] = UNCONNECTED;
						#ifdef debug
							sb_append_ch(dboard, " 0");
						#endif
						break;
					case 1:
						//if(col == width - 3) board[row][width - 1] = T_LEFT;
						board[row][col + 1] = UNCONNECTED;
						#ifdef debug
							sb_append_ch(dboard, " 1");
						#endif
						break;
					case 2:
						//if(row == height - 3) board[height - 1][col] = T_UP;
						board[row + 1][col] = UNCONNECTED;
						#ifdef debug
							sb_append_ch(dboard, " 2");
						#endif
						break;
					case 3:
						//if(row == 2) board[0][col] = T_RIGHT;
						board[row][col - 1] = UNCONNECTED;
						#ifdef debug
							sb_append_ch(dboard, " 3");
						#endif
						break;
					default:
						#ifdef debug
							sb_append_ch(dboard, " U");
						#endif
						break;
				}
				board[row][col] = UNCONNECTED;
				// board[row][col] = find_wall_type(board, row, col, height - 1, width - 1);
			}
		}

		#ifdef debug
			sb_append_ch(dboard, " ]\n");
		#endif
	}

	#ifdef debug
		char* dres = sb_concat(dboard);
		destroy_stringbuilder(dboard);
		printf("%s", dres);
		free(dres);
	#endif

	for(int row = 0; row < height; row++)
		for(int col = 0; col < width; col++)
			if(space_is_wall(board[row][col])) board[row][col] = find_wall_type(board, row, col, height - 1, width - 1);

	board[1][1] = START;
	board[height - 2][width - 2] = FINISH;

	return board;
}

unsigned* find_player(game g)
{
	if(g)
	{
		unsigned* result = calloc(2, sizeof(unsigned));
		if(!result) err(1, "Memory Error");

		for(int row = 0; row < g->board_height; row++)
			for(int col = 0; col < g->board_width; col++)
				if(g->board[row][col] == PLAYER)
				{
					result[0] = row;
					result[1] = col;
					break;
				}

		return result;
	}

	return 0;
}

unsigned char game_is_win(game g)
{
	if(g)
	{
		unsigned* position = find_player(g);

		if(position[0] && position[1] && position[0] == (g->board_height - 2) && position[1] == (g->board_width - 2))
		{
			free(position);
			return 1;
		}

		free(position);
	}

	return 0;
}

game take_turn(game g, move_direction direction)
{
	unsigned* position = find_player(g);
	// unsigned char valid = 0;

	switch(direction)
	{
		case UP:
			if(position[0] > 1 && !space_is_wall(g->board[position[0] - 1][position[1]]))
			{
				// g->board[position[0]][position[1]] = EMPTY;
				g->board[position[0] - 1][position[1]] = PLAYER;

				switch(g->p->previous_move)
				{
					case DOWN:
					case UP:
						g->board[position[0]][position[1]] = PVERTICAL;
						break;
					case LEFT:
						g->board[position[0]][position[1]] = PCORNER_U_RIGHT;
						break;
					case RIGHT:
						g->board[position[0]][position[1]] = PCORNER_U_LEFT;
						break;
				}

				g->p->previous_move = direction;
			}
			break;
		case LEFT:
			if(position[1] > 1 && !space_is_wall(g->board[position[0]][position[1] - 1]))
			{
				// g->board[position[0]][position[1]] = EMPTY;
				g->board[position[0]][position[1] - 1] = PLAYER;

				switch(g->p->previous_move)
				{
					case DOWN:
						g->board[position[0]][position[1]] = PCORNER_U_LEFT;
						break;
					case UP:
						g->board[position[0]][position[1]] = PCORNER_LEFT;
						break;
					case RIGHT:
					case LEFT:
						g->board[position[0]][position[1]] = PHORIZONTAL;
						break;
				}

				g->p->previous_move = direction;
			}
			break;
		case RIGHT:
			if(position[1] < g->board_width - 1 && !space_is_wall(g->board[position[0]][position[1] + 1]))
			{
				// g->board[position[0]][position[1]] = EMPTY;
				g->board[position[0]][position[1] + 1] = PLAYER;

				switch(g->p->previous_move)
				{
					case DOWN:
						g->board[position[0]][position[1]] = PCORNER_U_RIGHT;
						break;
					case UP:
						g->board[position[0]][position[1]] = PCORNER_RIGHT;
						break;
					case RIGHT:
					case LEFT:
						g->board[position[0]][position[1]] = PHORIZONTAL;
						break;
				}

				g->p->previous_move = direction;
			}

			g->p->good_move_count++; // Player made a good move
			break;
		case DOWN:
			if(position[0] < g->board_height - 1 && !space_is_wall(g->board[position[0] + 1][position[1]]))
			{
				// g->board[position[0]][position[1]] = EMPTY;
				g->board[position[0] + 1][position[1]] = PLAYER;

				switch(g->p->previous_move)
				{
					case DOWN:
					case UP:
						g->board[position[0]][position[1]] = PVERTICAL;
						break;
					case LEFT:
						g->board[position[0]][position[1]] = PCORNER_RIGHT;
						break;
					case RIGHT:
						g->board[position[0]][position[1]] = PCORNER_LEFT;
						break;
				}

				g->p->previous_move = direction;
			}

			g->p->good_move_count++; // Player made a good move
			break;
		default:
			err(5, "Invalid move direction");
	}

	if(position[0] == 1 && position[1] == 1) g->board[1][1] = START;

	g->turn++;

	return g;
}

game score_round(game g)
{
	if(g)
	{
		size_t diagonal = g->board_width + g->board_height;
		diagonal *= 1.10;

		time_t time_diff = time(0) - g->start_time;

		g->p->score += abs((((double)diagonal) / (((double)g->p->good_move_count) / (double)diagonal)) / (double)time_diff);
	}

	return g;
}
