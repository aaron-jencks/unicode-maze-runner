#pragma once

#include "gamedefs.h"


void clear_screen();

char* boxed_text(const char* content);

void pause_term();

char* board_string(board_space** board, unsigned rstart, unsigned cstart, unsigned width, unsigned height);

size_t* get_screen_size();
