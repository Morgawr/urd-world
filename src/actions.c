#include <stdio.h>
#include <ctype.h>
#include <libtelnet.h>
#include <gamedata.h>
#include <urd_macros.h>
#include <urd_interface.h>
#include <string.h>
#include <unistd.h>

void print_description(struct urd_status *game)
{
	sprintf(game->output, "%s", game->state.description);
}

/* Print help message */
static void print_help(struct urd_status *game)
{
	char *message = 
		" These are the available commands for Urd world:\n"
		"\thelp - Displays this message.\n"
		"\trepeat - Displays the current action's description.\n"
		"\tnext - Continues with the game.";

	sprintf(game->output, "%s", message);
}

static void print_repeat(struct urd_status *game)
{
	sprintf(game->output, "%s", game->state.description);
}

/* Function that checks for default commands like
 * help, repeat, next.
 * Returns 0 if no default command was received, 1 if it needs to be ignored
 * or 2 if it's next (aka game should "continue").
 */
int parse_defaults(struct urd_status *game)
{
	const char *c;
	for(c = game->command; !isspace(c[0]) && c[0] != '\0'; c++);
		/* Skipping for body ... */

	int length = c - game->command + 1;
	char cmd[length];
	strncpy(cmd, game->command, length);
	cmd[length-1] = '\0';

	if(strncmp(cmd, "help", length) == 0) {
		print_help(game);
		return 1;
	}
	if(strncmp(cmd, "repeat", length) == 0) {
		print_repeat(game);
		return 1;
	}
	if(strncmp(cmd, "next", length) == 0) {
		return 2;
	}

	return 0;
}


