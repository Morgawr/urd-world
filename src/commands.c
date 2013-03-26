#include <stdio.h>
#include <libtelnet.h>
#include <gamedata.h>
#include <urd_macros.h>
#include <urd_interface.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


struct cmd_stack* init_cmd_stack()
{
	struct cmd_stack *s = malloc(sizeof(s));
	s->bottom = NULL;
	s->top = NULL;
	s->args = 0;
	return s;
}

/* msg will get copied over */
void push_cmd_stack(struct cmd_stack *s, const char *msg, size_t length)
{
	struct cmd_elem *el = malloc(sizeof(el));
	el->msg = malloc(length+1);
	strncpy(el->msg, msg, length);
	el->msg[length] = '\0';

	if(s->args == 0) { /* first element in stack! */
		el->next = NULL;
		s->top = s->bottom = el;
		s->args++;
		return;
	}

	el->next = s->bottom;
	s->bottom = el;
	s->args++;
}

/* The one performing the pop operation will have to free the string */
char* pop_cmd_stack(struct cmd_stack *s)
{
	if(s->args == 0)
		return NULL;

	struct cmd_elem *el = s->bottom;
	s->bottom = el->next;
	char *msg = el->msg;
	free(el);
	s->args--;
	if(s->args == 0)
		s->top = s->bottom = NULL;
	return msg;
}

/* The returned message is just a copy and has to be freed! */
char* peek_cmd_stack(struct cmd_stack *s)
{
	if(s->args == 0)
		return NULL;
	int length = strlen(s->bottom->msg)+1;
	char *msg = malloc(length);
	strncpy(msg, s->bottom->msg, length);
	msg[length-1] = '\0'; /* just to be sure */
	return msg;
}

void free_cmd_stack(struct cmd_stack *s)
{
	while(s->args) {
		char *data = pop_cmd_stack(s);
		free(data);
	}
	free(s);
}

static int is_ignored_word(const char *word, size_t length)
{
	/* First and foremost, check for string of only spaces */
	int onlyspaces = 1;
	for(size_t i = 0; i < length; i++) {
		if(!isspace(word[i])) {
			onlyspaces = 0;
			break;
		}
	}

	if(onlyspaces)
		return 1;

	return 0;
}


struct cmd_stack* obtain_command(const char *cmd, size_t length)
{
	struct cmd_stack *s = init_cmd_stack();
	
	const char *ptr = cmd;
	unsigned int counter = 0;
	while(ptr != '\0' && counter < length) {
		counter++;
		ptr++;
	}
	
	if(ptr[0] != '\0')
		fprintf(stderr, "WARNING - possible buffer overflow attempt\n");

	unsigned int idx = counter;
	for(int i = counter; i >= 0; i--, ptr--) {
		if(ptr[0] == ' ' || ptr == cmd) {
			if(!is_ignored_word(ptr, idx - i)) {
				push_cmd_stack(s, ptr, idx - i);
			}
			idx = i;
		}
	}
	printf("%d\n",s->args);

	return s;
}

