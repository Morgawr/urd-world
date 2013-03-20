#include <stdio.h>
#include <libtelnet.h>
#include <gamedata.h>
#include <urd_macros.h>
#include <urd_interface.h>
#include <string.h>
#include <unistd.h>

/* Print the welcome message to the output buffer */
void urd_welcome(struct urd_status *game)
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
