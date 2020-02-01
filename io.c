#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "io.h"
#include "utf8.h"


char getkey()
{
	#ifndef debug
		size_t slen = u8_strlen("\\u001b[8m");
		char* buf = calloc(slen + 1, sizeof(char));
		u8_unescape(buf, slen, "\\u001b[8m");
		printf("%s", buf);
		free(buf);
	#endif

	struct termios attr;
	tcgetattr(0, &attr);
	attr.c_lflag &= ~ICANON;
	tcsetattr(0, TCSANOW, &attr);

	fflush(stdin);

	int c;
	while((c = getchar()) == EOF);

	attr.c_lflag |= ICANON;
	tcsetattr(0, TCSANOW, &attr);

	fflush(stdin);

	#ifndef debug
		slen = u8_strlen("\\u001b[28m\r");
		buf = calloc(slen + 1, sizeof(char));
		u8_unescape(buf, slen, "\\u001b[28m\r");
		printf("%s", buf);
		free(buf);
	#endif

	return c;
}
