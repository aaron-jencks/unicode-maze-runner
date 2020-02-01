#include <stdlib.h>
#include <time.h>

#include "rand_util.h"

unsigned char seed_set = 0;

int randint(int min, int max)
{
	if(!seed_set)
	{
		seed_set = 1;
		srand(time(0));
	}

	return (rand() % ((max + 1) - min)) + min;
}
