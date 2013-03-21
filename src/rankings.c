#include <stdio.h>
#include <ctype.h>
#include <libtelnet.h>
#include <gamedata.h>
#include <urd_macros.h>
#include <urd_interface.h>
#include <string.h>
#include <unistd.h>


void urd_update_rankings(struct urd_status *game)
{
	int res = parse_defaults(game);

	if(res == 1)
		return;

	if(res == 2) {
		//need to go back to beginning
		return;
	}

	sprintf(game->output, "Invalid command.\n");
}
