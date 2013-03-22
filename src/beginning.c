#include <stdio.h>
#include <ctype.h>
#include <libtelnet.h>
#include <gamedata.h>
#include <urd_macros.h>
#include <urd_interface.h>
#include <string.h>
#include <unistd.h>

static void print_welcome(struct urd_status *game);

void generate_begin(struct urd_status *game)
{
	memset(&(game->state), 0, sizeof(game->state));
	game->state.base.type = URD_BEGIN;
	game->state.begin.intro_message = 1;
	print_welcome(game);
	print_description(game);
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
		"\t3) Read the tutorial\n"
		"\t4) Approach the board of rankings\n"
		"\t5) I'm too scared, get me out of here!\n"
		"\n"
		"Choose your poison, oh mighty player.";
	sprintf(game->state.base.description, "%s", message);
}

void urd_update_begin(struct urd_status *game)
{
	if(!game->state.begin.intro_message) {
		game->state.begin.intro_message = 1;
		print_description(game);
		return;
	}

	int res = parse_defaults(game);

	if(res == 1)
		return;

	if(res == 2) {
		sprintf(game->output, "We received next!");
	}
	else {
		switch(game->command[0]) {
			case '1':
				sprintf(game->output,
						"Not implemented yet.");
				break;
			case '2':
				sprintf(game->output,
						"Not implemented yet.");
				break;
			case '3':
				sprintf(game->output,
						"Tutorial doesn't exist yet.");
				break;
			case '4':
				generate_rankings(game);
				break;
			case '5':
				sprintf(game->output, 
						"What a crybaby... are you "
						"really quitting?\n\n"
						"Oh well, I can't force you "
						"to stay.\n");
				game->exiting = 1;
				break;
			default:
				sprintf(game->output, "Command not "
						"recognized");
		}
	}

}
