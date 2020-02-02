#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>

#include "utf8.h"
#include "player.h"
#include "game.h"
#include "stringbuilder.h"
#include "gamedefs.h"
#include "display.h"
#include "io.h"

void display_stats(game g)
{
	printf("Round %lu:\nPlayer: %s\nScore: %lu\nMoves: %lu\nElapsed Time: %ld sec\n", g->round, g->p->name, g->p->score, g->turn, time(0) - g->start_time);
}


int main() {
	setlocale(LC_ALL, "UTF-8");
	clear_screen();

	char *pname;
	pname = readline("Please enter a name for player 1: ");

	player p = create_player(pname);
	free(pname);

	game g = create_game(p);

	// Input loop
	unsigned char ending = 0;
	while(!ending)
	{
		clear_screen();
		char* b = board_string(g->board, 0, 0, g->board_width, g->board_height);
		printf("%s", b);
		free(b);

		display_stats(g);

		char c = getkey();
		switch(c)
		{
			case 'a':
				take_turn(g, LEFT);
				break;
			case 's':
				take_turn(g, DOWN);
				break;
			case 'd':
				take_turn(g, RIGHT);
				break;
			case 'w':
				take_turn(g, UP);
				break;
			default:
				printf("Unknown keystroke, use AWSD to move\n");
				pause_term();
				break;
		}

		if(game_is_win(g))
		{
			g->round++;
			score_round(g);
			clear_screen();
			display_stats(g);
			pause_term();
			game_reset(g);	// Regenerates the maze and increases the round
		}
	}

	// Cleanup
	destroy_game(g);
	destroy_player(p);
}
