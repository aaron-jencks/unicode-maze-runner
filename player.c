#include <stdlib.h>
#include <string.h>

#include "player.h"
#include "gamedefs.h"


player create_player(const char* name)
{
	player p = calloc(1, sizeof(player_t));
	if(!p) exit(1);

	size_t nsize = strlen(name);
	p->name = calloc(nsize + 1, sizeof(char));
	if(!p->name) exit(1);

	p->name = memcpy(p->name, name, nsize);

	p->previous_move = NONE;

	return p;
}


void destroy_player(player p)
{
	if(p)
	{
		if(p->name)
			free(p->name);
		free(p);
	}
}
