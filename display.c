#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

// For terminal dimensions
#include <sys/ioctl.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include "display.h"
#include "utf8.h"
#include "stringbuilder.h"


void clear_screen()
{
	size_t slen = u8_strlen("\\u001b[2J\\u001b[H");
	char* buf = calloc(slen + 1, sizeof(char));
	u8_unescape(buf, slen + 1, "\\u001b[2J\\u001b[H");
	printf("%s", buf);
	free(buf);
}

char* boxed_text(const char* content)
{
	if(content)
	{
		stringbuilder lines = create_stringbuilder(), result = create_stringbuilder();

		char* buf = calloc(strlen(content) + 1, sizeof(char));
		if(!buf) exit(1);

		buf = memcpy(buf, content, strlen(content));

		char *start = buf, *c = buf, found = 0;
		for(c = buf; *c; c++) if(*c == '\n')
		{
			found = 1;
			*c = 0;
			if(start != c)
			{
				sb_append_ch(lines, start);
				start = c + 1;
			}
		}

		if(!found) sb_append_ch(lines, start);
		free(buf);

		size_t max_width = 0;
		for(sb_node sc  = lines->head; sc; sc = sc->next) if(strlen(sc->value) > max_width) max_width = strlen(sc->value);

		max_width += 2;

		for(int i = 0; i < max_width + 2; i++)
			if(i == 0 || i == max_width + 1) sb_append_ch(result, (i) ? "\\u2510\n" : "\\u250c");
			else sb_append_ch(result, "\\u2500");

		for(sb_node line = lines->head; line; line = line->next)
		{
			sb_append_ch(result, "\\u2502 ");
			sb_append_ch(result, line->value);

			size_t slen = strlen(line->value);
			size_t diff = max_width - slen - 1;
			for(int i = 0; i < diff; i++) sb_append_ch(result, " ");
			sb_append_ch(result, "\\u2502\n");
		}

		for(int i = 0; i < max_width + 2; i++)
			if(i == 0 || i == max_width + 1) sb_append_ch(result, (i) ? "\\u2518\n" : "\\u2514");
			else sb_append_ch(result, "\\u2500");

		destroy_stringbuilder(lines);

		buf = sb_concat(result);
		destroy_stringbuilder(result);

		char* newbuf = calloc(u8_strlen(buf) + 1, sizeof(char));
		if(!newbuf) err(1, "Memory error");

		u8_unescape(newbuf, u8_strlen(buf), buf);
		free(buf);

		return newbuf;
	}
}

void pause_term()
{
	char* l = readline("Press any key to continue");
	free(l);
}

char* board_string(board_space** board, unsigned rstart, unsigned cstart, unsigned width, unsigned height)
{
	if(board)
	{
		stringbuilder result = create_stringbuilder();

		#ifdef debug
			stringbuilder ddebug = create_stringbuilder();
		#endif

		for(int row = rstart; row < height; row++)
		{
			#ifdef debug
				sb_append_ch(ddebug, "[");
			#endif

			for(int col = cstart; col < width; col++)
			{
				#ifdef debug
					sb_append_ch(ddebug, " ");
				#endif

				switch(board[row][col])
				{
					case HORIZONTAL:
						sb_append_ch(result, "\\u2500");
						#ifdef debug
							sb_append_ch(ddebug, "H");
						#endif
						break;
					case VERTICAL:
						sb_append_ch(result, "\\u2502");
						#ifdef debug
							sb_append_ch(ddebug, "V");
						#endif
						break;
					case CORNER_RIGHT:
						sb_append_ch(result, "\\u250c");
						#ifdef debug
							sb_append_ch(ddebug, "CR");
						#endif
						break;
					case CORNER_LEFT:
						sb_append_ch(result, "\\u2510");
						#ifdef debug
							sb_append_ch(ddebug, "CL");
						#endif
						break;
					case CORNER_U_RIGHT:
						sb_append_ch(result, "\\u2514");
						#ifdef debug
							sb_append_ch(ddebug, "CUR");
						#endif
						break;
					case CORNER_U_LEFT:
						sb_append_ch(result, "\\u2518");
						#ifdef debug
							sb_append_ch(ddebug, "CUL");
						#endif
						break;
					case UNCONNECTED:
						sb_append_ch(result, "\\u22c5");
						#ifdef debug
							sb_append_ch(ddebug, "U");
						#endif
						break;
					case T_UP:
						sb_append_ch(result, "\\u2534");
						#ifdef debug
							sb_append_ch(ddebug, "TU");
						#endif
						break;
					case T_RIGHT:
						sb_append_ch(result, "\\u251c");
						#ifdef debug
							sb_append_ch(ddebug, "TR");
						#endif
						break;
					case T_LEFT:
						sb_append_ch(result, "\\u2524");
						#ifdef debug
							sb_append_ch(ddebug, "TL");
						#endif
						break;
					case T_DOWN:
						sb_append_ch(result, "\\u252c");
						#ifdef debug
							sb_append_ch(ddebug, "TD");
						#endif
						break;
					case CENTER:
						sb_append_ch(result, "\\u253c");
						#ifdef debug
							sb_append_ch(ddebug, "C");
						#endif
						break;
					case EMPTY:
						sb_append_ch(result, " ");
						#ifdef debug
							sb_append_ch(ddebug, "E");
						#endif
						break;
					case PLAYER:
						sb_append_ch(result, "\\u001b[95m\\u2022\\u001b[0m");
						#ifdef debug
							sb_append_ch(ddebug, "P");
						#endif
						break;
					case START:
						sb_append_ch(result, "\\u001b[92m\\u2022\\u001b[0m");
						#ifdef debug
							sb_append_ch(ddebug, "S");
						#endif
						break;
					case FINISH:
						sb_append_ch(result, "\\u001b[91m\\u2022\\u001b[0m");
						#ifdef debug
							sb_append_ch(ddebug, "F");
						#endif
						break;
					case PVERTICAL:
						sb_append_ch(result, "\\u001b[95m\\u2506\\u001b[0m");
						break;
					case PHORIZONTAL:
						sb_append_ch(result, "\\u001b[95m\\u2504\\u001b[0m");
						break;
					case PCORNER_RIGHT:
						sb_append_ch(result, "\\u001b[95m\\u250c\\u001b[0m");
						break;
					case PCORNER_LEFT:
						sb_append_ch(result, "\\u001b[95m\\u2510\\u001b[0m");
						break;
					case PCORNER_U_RIGHT:
						sb_append_ch(result, "\\u001b[95m\\u2514\\u001b[0m");
						break;
					case PCORNER_U_LEFT:
						sb_append_ch(result, "\\u001b[95m\\u2518\\u001b[0m");
						break;
					default:
						err(6, "Invalid board symbol found");
				}
			}
			sb_append_ch(result, "\n");

			#ifdef debug
				sb_append_ch(ddebug, " ]\n");
			#endif
		}

		char* real_result = sb_concat(result);
		destroy_stringbuilder(result);

		char* buf = calloc(u8_strlen(real_result) + 1, sizeof(char));
		if(!buf) err(1, "Memory error");

		u8_unescape(buf, u8_strlen(real_result), real_result);
		free(real_result);

		#ifdef debug

			char* dreal_result = sb_concat(ddebug);
			destroy_stringbuilder(ddebug);

			char* dbuf = calloc(u8_strlen(dreal_result) + 1, sizeof(char));
			if(!dbuf) err(1, "Memory error");

			u8_unescape(dbuf, u8_strlen(dreal_result), dreal_result);
			free(dreal_result);

			printf("%s", dbuf);
			free(dbuf);

		#endif

		return buf;
	}

	return 0;
}

size_t* get_screen_size()
{
	size_t* result = malloc(sizeof(size_t) * 2);
	if(!result) err(1, "Memory error");

	struct winsize ws;
	int fd;

	fd = open("/dev/tty", O_RDWR);
	if(fd < 0)
		err(8, "/dev/tty");

	if(ioctl(fd, TIOCGWINSZ, &ws) < 0)
		err(8, "/dev/tty");

	result[1] = ws.ws_row;
	result[0] = ws.ws_col;

	close(fd);

	return result;
}
