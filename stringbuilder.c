#include <stdlib.h>
#include <string.h>

#include "stringbuilder.h"
#include "utf8.h"


sb_node create_node(const char* value)
{
	sb_node n = calloc(1, sizeof(sb_node_t));
	if(!n) exit(1);

	size_t slen = strlen(value);
	n->value = calloc(slen + 1, sizeof(char));
	if(!n->value) exit(1);

	n->value = memcpy(n->value, value, sizeof(char) * slen);

	return n;
}


void destroy_node(sb_node n)
{
	if(n)
	{
		if(n->value) free(n->value);
		free(n);
	}
}


void sb_clear(stringbuilder sb)
{
	if(sb && sb->count)
	{
		sb_node c = sb->head;
		while(c)
		{
			sb_node temp = c->next;
			if(c->next) c->next->previous = 0;

			if(c->previous) c->previous->next = 0;
			destroy_node(c);
			c = temp;
		}

		sb->head = 0;
		sb->tail = 0;
		sb->count = 0;
	}
}


size_t sb_len(stringbuilder sb)
{
	if(sb)
	{
		size_t count = 0;
		for(sb_node c = sb->head; c; c = c->next) count += strlen(c->value);
		return count;
	}
	return 0;
}


stringbuilder create_stringbuilder()
{
	stringbuilder sb = calloc(1, sizeof(stringbuilder_t));
	if(!sb) exit(1);
	return sb;
}


void destroy_stringbuilder(stringbuilder sb)
{
	if(sb)
	{
		sb_clear(sb);
		free(sb);
	}
}


//stringbuilder sb_append(stringbuilder sb, u_int32_t* str)
//{
//	if(sb && str)
//	{
//		sb_node n = create_node(str);
//		if(sb->tail) sb->tail->next = n;
//		n->previous = sb->tail;
//		sb->tail = n;
//		if(!sb->count++) sb->head = n;
//	}
//}


stringbuilder sb_append_ch(stringbuilder sb, const char* str)
{
	if(sb && str)
	{
		sb_node n = create_node(str);
		if(sb->tail) sb->tail->next = n;
		n->previous = sb->tail;
		sb->tail = n;
		if(!sb->count++) sb->head = n;
	}

	return sb;
}


char* sb_concat(stringbuilder sb)
{
	if(sb)
	{
		char* result = calloc(sb_len(sb) + 1, sizeof(char));
		if(!result) exit(1);
		char* temp = result;
		size_t slen;

		sb_node c = sb->head;
		while(c)
		{
			slen = strlen(c->value);
			memcpy(temp, c->value, slen);
			temp += slen;
			c = c->next;
		}

		return result;
	}

	return 0;
}

char** sb_arr(stringbuilder sb)
{
	if(sb)
	{
		char** res = malloc(sizeof(char*) * (sb->count + 1));
		if(!res) exit(1);
		res[sb->count] = 0;

		for(sb_node c = sb->head; c; c = c->next) *(res++) = c->value;

		return res;
	}

	return 0;
}
