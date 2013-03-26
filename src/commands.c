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

static int is_ignored_word(const char *word, size_t length,
				struct ignored_words *iw)
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

	struct ignored_word *el = iw->first;

	char *ww = malloc(length+1);
	strncpy(ww, word, length);
	ww[length] = '\0';
	while(el != NULL) {
		if(strncmp(word, el->word, length) == 0)
			return 1;
		el = el->next;
	}

	return 0;
}


struct cmd_stack* obtain_command(const char *cmd, size_t length,
					struct ignored_words *iw) 
{
	struct cmd_stack *s = init_cmd_stack();
	
	const char *ptr = cmd;
	unsigned int counter = 0;
	while(ptr != '\0' && counter < length) {
		/* find our first \n or \r, everything beyond that is moot */
		if(ptr[0] == '\n' || ptr[0] == '\r')
			break;
		counter++;
		ptr++;
	
	}
	
	if(ptr[0] != '\0' && ptr[0] != '\n' && ptr[0] != '\r')
		fprintf(stderr, "WARNING - possible buffer overflow attempt\n");

	unsigned int idx = counter;
	for(int i = counter; i >= 0; i--, ptr--) {
		const char *newp = ptr;
		int newlength = idx - i;
		if(newp[0] == ' ' || newp == cmd) {
			if(newp[0] == ' ') {
				newp++;
				newlength--;
			}
			if(!is_ignored_word(newp, newlength, iw)) {
				push_cmd_stack(s, newp, newlength);
			}
			idx = i;
		}
	}

	return s;
}

struct ignored_words* init_ignored_words(sqlite3 *db)
{
	struct ignored_words *iw = malloc(sizeof(iw));
	iw->first = NULL;
	iw->count = 0;

	sqlite3_stmt *res;
	const char *tail;

	int err = sqlite3_prepare_v2(db, 
			"SELECT word FROM ignored_words WHERE ignored='TRUE'",
			1000, &res, &tail);

	
	if(err != SQLITE_OK) {
		fprintf(stderr, "Error processing ignored words.\n");
		fprintf(stderr, "Game will have empty ignored word list!\n");
		return iw;
	}

	while(sqlite3_step(res) == SQLITE_ROW) {
		add_ignored_word(iw, (char *)sqlite3_column_text(res, 0));
	}

	sqlite3_finalize(res);

	return iw;
}

void add_ignored_word(struct ignored_words *iw, const char *word)
{
	struct ignored_word *el = malloc(sizeof(el));
	struct ignored_word *it = iw->first;

	if(iw->first != NULL)
		for(; it->next != NULL; it = it->next) ; /* empty body */

	if(iw->first == NULL) {
		iw->first = el;
	}
	else {
		it->next = el;
	}

	el->next = NULL;
	int length = strlen(word)+1;
	el->word = malloc(length);
	strncpy(el->word, word, length-1);
	el->word[length-1] = '\0';
	iw->count++;
}

void drop_ignored_word(struct ignored_words *iw)
{
	if(iw->count == 0 || iw->first == NULL)
		return;

	struct ignored_word *el = iw->first;
	iw->first = iw->first->next;
	free(el->word);
	free(el);
	iw->count--;
}

void free_ignored_words(struct ignored_words *iw)
{
	while(iw->count != 0)
		drop_ignored_word(iw);
	free(iw);
}

