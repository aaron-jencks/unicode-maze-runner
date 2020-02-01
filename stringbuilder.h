#pragma once

#include <stdlib.h>


typedef struct _sb_node_t {
	struct _sb_node_t* next;
	struct _sb_node_t* previous;
	char* value;
} sb_node_t;

typedef sb_node_t* sb_node;


typedef struct _stringbuilder_t {
	sb_node head;
	sb_node tail;
	size_t count;
} stringbuilder_t;

typedef stringbuilder_t* stringbuilder;


stringbuilder create_stringbuilder();

void destroy_stringbuilder(stringbuilder sb);

//stringbuilder sb_append(stringbuilder sb, u_int32_t* str);
stringbuilder sb_append_ch(stringbuilder sb, const char* str);

char* sb_concat(stringbuilder sb);

char** sb_arr(stringbuilder sb);
