#include <stdio.h>
#include <ctype.h>
#include <libtelnet.h>
#include <gamedata.h>
#include <urd_macros.h>
#include <urd_interface.h>
#include <string.h>
#include <unistd.h>

/* Print help message */
static void print_help(struct urd_status *game)
{
	char *message = 
		" These are the available commands for Urd world:\n"
		"\thelp - Displays this message.\n"
		"\trepeat - Re-prints the previous message.\n"
		"\tnext - Continues with the game.\n";

	sprintf(game->output, "%s", message);
}

static void print_repeat(struct urd_status *game)
{
	sprintf(game->output, "%s", game->snd_output);
}

/* Function that checks for default commands like
 * help, repeat, next.
 * Returns 0 if no default command was received, 1 if it needs to be ignored
 * or 2 if it's next (aka game should "continue").
 */
static int parse_defaults(struct urd_status *game)
{
	const char *c;
	for(c = game->command; !isspace(c[0]) && c[0] != '\0'; c++);
		/* Skipping for body ... */

	int length = c - game->command + 1;
	char cmd[length];
	strncpy(cmd, game->command, length);
	cmd[length-1] = '\0';

	DBG(("..%s..",cmd));

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

/* Print the welcome message to the output buffer */
static void print_welcome(struct urd_status *game)
{
	char *message = 
		"  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
		" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n"
		"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
		"Welcome to the world of Urd!\n"
		"What is Urd, you may ask?\n"
		"Urd is the dungeon where adventurers go to die.\n"
		"Urd is the place where adventures find their end.\n"
		"You won't find fame and fortune in here, just suffering,\n"
		"pain and a simple escape from boredom. You dare to take a\n"
		"peek?\n"
		"\n"
		"\t1) Start a new adventure\n"
		"\t2) Load previous adventure\n"
		"\t3) Approach the board of rankings\n"
		"\t4) I'm too scared, get me out of here!\n"
		"\n"
		"Choose your poison, oh mighty player.";
	sprintf(game->output, "%s", message);
}

void urd_update_begin(struct urd_status *game)
{
	if(!game->state.begin.intro_message) {
		print_welcome(game);
		game->state.begin.intro_message = 1;
		return;
	}

	int res = parse_defaults(game);

	if(res == 1)
		return;

	if(res == 2) {
		sprintf(game->output, "%s", "We received next!\n");
	}
	else {
		switch(game->command[0]) {
			case '1':
				sprintf(game->output,
						"Not implemented yet.\n");
				break;
			case '2':
				sprintf(game->output,
						"Not implemented yet.\n");
				break;
			case '3':
				sprintf(game->output,
						"Not implemented yet.\n");
				break;
			case '4':
				sprintf(game->output, 
						"What a crybaby... are you "
						"really quitting?\n\n"
						"Oh well, I can't force you "
						"to stay.\n\n"
						"\t. . .\n\n"
						"Just kidding, quitting is "
						"not implemented yet.\n\n"
						"WE SAO NOW!\n");
				break;
			default:
				sprintf(game->output, "Command not "
						"recognized\n");
		}
	}

}
