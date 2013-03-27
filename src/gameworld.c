#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <libtelnet.h>
#include <sqlite3.h>
#include <urd_macros.h>
#include <gamedata.h>
#include <urd_interface.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

static const telnet_telopt_t opts[] = {
	{ TELNET_TELOPT_ECHO,      TELNET_WONT, TELNET_DONT },
	{ TELNET_TELOPT_TTYPE,     TELNET_WONT, TELNET_DONT },
	{ TELNET_TELOPT_COMPRESS2, TELNET_WONT, TELNET_DONT },
	{ TELNET_TELOPT_ZMP,       TELNET_WONT, TELNET_DONT },
	{ TELNET_TELOPT_MSSP,      TELNET_WONT, TELNET_DONT },
	{ TELNET_TELOPT_BINARY,    TELNET_WONT, TELNET_DONT },
	{ TELNET_TELOPT_NAWS,      TELNET_WONT, TELNET_DONT },
	{ TELNET_TELOPT_LINEMODE,  TELNET_WONT, TELNET_DONT },
	{ -1, 0, 0 }
};

static void handle_iac(unsigned char cmd, struct telnet_data *t_data)
{
	/* This function was supposed to handle IAC commands. I don't really
	 * want to bother with it at the moment so... nope
	 */
}

static void urd_update(struct urd_status *game)
{

	if(game->state.cmd != NULL)
		fprintf(stderr, "WARNING - possible memory corruption\n");

	game->state.cmd = obtain_command(game->command, game->command_size,
					game->ignored_words);

	switch(game->state.type) {
		case URD_BEGIN:
			urd_update_begin(game);
			break;
		case URD_CREATION:
			break;
		case URD_DUNGEON:
			break;
		case URD_BATTLE:
			break;
		case URD_RANKINGS:
			urd_update_rankings(game);
			break;
		default:
			DBG(("Unhandled game state\n"));
	}

	free_cmd_stack(game->state.cmd);
	game->state.cmd = NULL;

	/* Need to create a parsing function that turns the user's input into
	 * a series of commands that can be easily parsed
	 */

	/* A command is a sequence of words starting from the first which is a
	 * verb. Depending on the verb, the command's structure will act on
	 * characters, enemies or items.
	 */
}

static void
process_input(const char *buffer, size_t size, struct telnet_data *t_data)
{
	DBG(("#%d Conn - %s\n",t_data->sock, buffer));
	if(buffer[0] == '\n' || buffer[0] == '\r')
		return;
	t_data->game.command = malloc(size+1);
	strncpy(t_data->game.command, buffer, size);
	t_data->game.command[size] = '\0';

	for(size_t i = 0; i < size; i++) {
		t_data->game.command[i] = tolower(t_data->game.command[i]);
	}

	t_data->game.command_size = size;
	memset(t_data->game.output, 0, MAX_REPLY); /* clean extra stuff */

	urd_update(&t_data->game); /* Update state of the game */

	free(t_data->game.command);

	DBG(("#%d Reply - %s\n",t_data->sock, t_data->game.output));
	telnet_printf(t_data->telnet, "%s\n", t_data->game.output);
	telnet_printf(t_data->telnet, " > ");

}

static void
urd_reply(const char *buffer, size_t size, struct telnet_data *t_data)
{
	if(strlen(buffer) <= 0)
		return;
	send(t_data->sock, buffer, size, MSG_DONTWAIT);
}

static void handle_telnet(telnet_t *telnet, telnet_event_t *event, void *data)
{
	struct telnet_data *t_data = (struct telnet_data *)data;
	switch(event->type) {
		case TELNET_EV_DATA: 
			{
				struct data_t *op = (struct data_t *)event;
				process_input(op->buffer, op->size, t_data);
			}
			break;
		case TELNET_EV_SEND:
			{
				struct data_t *op = (struct data_t *)event;
				urd_reply(op->buffer, op->size, t_data);
			}
			break;
		case TELNET_EV_IAC:
			handle_iac(((struct iac_t *)event)->cmd, t_data);
			break;

		/* All the following commands are really irrelevant, depends on
		 * the telnet client but at the moment I won't care about it */
		case TELNET_EV_WILL:
			fprintf(stderr,"Got TELNET_EV_WILL, skipping.\n");
			break;
		case TELNET_EV_WONT:
			fprintf(stderr,"Got TELNET_EV_WONT, skipping.\n");
			break;
		case TELNET_EV_DO:
			fprintf(stderr,"Got TELNET_EV_DO, skipping.\n");
			break;
		case TELNET_EV_DONT:
			fprintf(stderr,"Got TELNET_EV_DONT, skipping.\n");
			break;
		case TELNET_EV_SUBNEGOTIATION:
			fprintf(stderr,"Got TELNET_EV_SUBNEGOTIATION, "
					"skipping.\n");
			break;
		case TELNET_EV_COMPRESS:
			fprintf(stderr,"Got TELNET_EV_COMPRESS, skipping.\n");
			break;
		case TELNET_EV_ZMP:
			fprintf(stderr,"Got TELNET_EV_ZMP, skipping.\n");
			break;
		case TELNET_EV_TTYPE:
			fprintf(stderr,"Got TELNET_EV_TTYPE, skipping.\n");
			break;
		case TELNET_EV_ENVIRON:
			fprintf(stderr,"Got TELNET_EV_ENVIRON, skipping.\n");
			break;
		case TELNET_EV_MSSP:
			fprintf(stderr,"Got TELNET_EV_MSSP, skipping.\n");
			break;
		case TELNET_EV_WARNING:
			fprintf(stderr,"Got TELNET_EV_WARNING, skipping.\n");
			break;
		case TELNET_EV_ERROR:
			fprintf(stderr,"Got TELNET_EV_ERROR, skipping.\n");
			break;
		default:
			fprintf(stderr,"Received unknown telnet event type. "
					"Skipping...\n");
	}
}

/* Initialize all the data for the beginning of the game. */
static int init_game(struct telnet_data *t_data)
{
	generate_begin(&(t_data->game));
	t_data->game.state.begin.intro_message = 0;
	t_data->game.party.first = t_data->game.party.current = NULL;
	t_data->game.party.avg_level = 0;

	/* Initialize sqlite instances */
	int err = sqlite3_open("data/words.db", &t_data->game.word_db);
	if(err) {
		fprintf(stderr, "Error initializing the word database\n");
		return -1;
	}

	t_data->game.ignored_words = init_ignored_words(t_data->game.word_db);
	t_data->game.state.cmd = NULL;
	return 0;
}


/* This is the main entry point in our world. Nothing is shared with the 
 * actual connection server so we don't need to ever go back there.
 */
_Noreturn void urd_main(int sockfd) 
{
	printf("Client #%d conneted.\n", sockfd);
	int ret;
	int bufsize = 4096;
	char buffer[bufsize];
	struct telnet_data t_data;
	t_data.sock = sockfd;
	t_data.telnet = telnet_init(opts, handle_telnet, 0, &t_data);
	t_data.game.exiting = 0;
	if(t_data.telnet == NULL) {
		fprintf(stderr,"Failed to init telnet for %d\n",t_data.sock);
		goto breakout;
	}
	ret = init_game(&t_data);
	if(ret < 0)
		goto breakout;
	strncpy(buffer,".",MAX_REPLY);
	telnet_recv(t_data.telnet, buffer, strlen(buffer));
	while(1) {
		ret = recv(t_data.sock, buffer, bufsize, 0);
		if (ret < 0) {
			fprintf(stderr,"Error when receiving message\n");
			perror("recv");
		}
		if (ret == 0) {
			break;
		}
		telnet_recv(t_data.telnet, buffer, ret);
		memset(buffer, 0, bufsize);
		if(t_data.game.exiting) {
			telnet_printf(t_data.telnet, 
					"\n\nIt is now possible to turn off the"
					" telnet client.\n");
			break;
		}
	}


breakout:;
	printf("Connection closed for %d\n",t_data.sock);
	sqlite3_close(t_data.game.word_db);
	telnet_free(t_data.telnet);
	close(sockfd);
	exit(0);
}
