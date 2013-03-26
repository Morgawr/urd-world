#include <stdio.h>
#include <ctype.h>
#include <libtelnet.h>
#include <gamedata.h>
#include <urd_macros.h>
#include <urd_interface.h>
#include <string.h>
#include <unistd.h>

void generate_rankings(struct urd_status *game)
{
	memset(&(game->state), 0, sizeof(game->state));
	game->state.type = URD_RANKINGS;
	sprintf(game->state.description, 
			"You approach the board of rankings. There seems to\n"
			"be a sign written on top. Here's what it says:\n"
			"\t\"The dungeon is not yet open to the public,\n"
			"\t to all aspiring adventurers, hold your\n"
			"\t horses. You shall be notified before\n"
			"\t the official opening.\"\n"
			"\t\t\tSigned: The King.\n\n"
			"You give up and back off, you're obviously not ready\n"
			"yet.\n"
			"\tProceed.");
	print_description(game);
}

void urd_update_rankings(struct urd_status *game)
{
	int res = parse_defaults(game);

	if(res == 1)
		return;

	if(res == 2) {
		generate_begin(game);
		return;
	}

	sprintf(game->output, "Invalid command.\n");
}
