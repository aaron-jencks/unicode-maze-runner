#include <stdlib.h>
#include <stdio.h>

#include "gamedefs.h"

unsigned char space_is_wall(board_space space)
{
	return (space == HORIZONTAL || space == VERTICAL || space == CENTER || space == UNCONNECTED ||
		space == CORNER_LEFT || space == CORNER_RIGHT || space == CORNER_U_RIGHT || space == CORNER_U_LEFT ||
		space == T_UP || space == T_DOWN || space == T_RIGHT || space == T_LEFT);
}

board_space find_wall_type(board_space** board, unsigned x, unsigned y, unsigned xmax, unsigned ymax)
{
	unsigned char xl = ((x) ? 1 : 0), xr = (x < xmax), yu = ((y) ? 1 : 0), yb = (y < ymax);
	unsigned char l = 0, u = 0, r = 0, d = 0;

	// Determine which sides have walls already

	// left/right
	if(xl && space_is_wall(board[x - 1][y])) u = 1;
	if(xr && space_is_wall(board[x + 1][y])) d = 1;

	// up/down
	if(yu && space_is_wall(board[x][y - 1])) l = 1;
	if(yb && space_is_wall(board[x][y + 1])) r = 1;

	// Determine the type of wall
	if(l && r && u && d) return CENTER;

	if(l)
		if(r)
			// We already ruled out the center, no need to check for it
			if(u) return T_UP;
			else if(d) return T_DOWN;
			else return HORIZONTAL;
		else
			if(u)
				if(d) return T_LEFT;
				else return CORNER_U_LEFT;
			else
				if(d) return CORNER_LEFT;
				else return HORIZONTAL; // left and no right still makes a horizontal dash
	else
		if(r)
			if(u)
				if(d) return T_RIGHT;
				else return CORNER_U_RIGHT;
			else
				if(d) return CORNER_RIGHT;
				else return HORIZONTAL; // right and no left still makes a horizontal dash
		else
			if(u || d) return VERTICAL; // There is nothing to the sides as long as one of the top or bottom has something, then we need a vertical wall
			else return UNCONNECTED;// There are not surrounding walls, it's just a column

	// This state is unreachable
	fprintf(stderr, "Invalid board state\n");
	exit(7);
}
