#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <libtelnet.h>
#include <urd_macros.h>
#include <urd_interface.h>
#include <string.h>
#include <unistd.h>

static const telnet_telopt_t opts[] = {
	{ TELNET_TELOPT_ECHO,      TELNET_WILL, TELNET_DO },
	{ TELNET_TELOPT_TTYPE,     TELNET_WONT, TELNET_DONT },
	{ TELNET_TELOPT_COMPRESS2, TELNET_WONT, TELNET_DONT },
	{ TELNET_TELOPT_ZMP,       TELNET_WONT, TELNET_DONT },
	{ TELNET_TELOPT_MSSP,      TELNET_WONT, TELNET_DONT },
	{ TELNET_TELOPT_BINARY,    TELNET_WONT, TELNET_DONT },
	{ TELNET_TELOPT_NAWS,      TELNET_WONT, TELNET_DONT },
	{ -1, 0, 0 }
};

struct urd_status {
	int exiting;
};

struct telnet_data {
	int sock;
	telnet_t *telnet;
	struct urd_status game;
};

void handle_iac(unsigned char cmd, struct telnet_data *t_data)
{
	/* This function was supposed to handle IAC commands. I don't really
	 * want to bother with it at the moment so... nope
	 */
	//if(cmd == 244) { /* Exit code */
	//	t_data->game.exiting = 1;
	//}
	//else {
	//	fprintf(stderr,"IAC got %d, unhandled.\n",cmd);
	//}
}

void process_input(const char *buffer, size_t size, struct telnet_data *t_data)
{
	printf("#%d Conn - %s",t_data->sock, buffer);
}

void urd_reply(const char *buffer, size_t size, struct telnet_data *t_data)
{
	/* I know I should deal with EV_SEND but I don't see any point */
}

void handle_telnet(telnet_t *telnet, telnet_event_t *event, void *data)
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


/* This is the main entry point in our world. Nothing is shared with the 
 * actual connection server so we don't need to ever go back there.
 */
void urd_main(int sockfd) 
{
	int ret;
	int bufsize = 4096;
	unsigned char buffer[bufsize];
	struct telnet_data t_data;
	t_data.sock = sockfd;
	t_data.telnet = telnet_init(opts, handle_telnet, 0, &t_data);
	t_data.game.exiting = 0;
	if(t_data.telnet == NULL) {
		fprintf(stderr,"Failed to init telnet for %d\n",t_data.sock);
		goto breakout;
	}
	while(1) {
		ret = recv(t_data.sock, buffer, bufsize, 0);
		if (ret < 0) {
			fprintf(stderr,"Error when receiving message\n");
			perror("recv");
		}
		telnet_recv(t_data.telnet, buffer, ret);
		memset(buffer, 0, bufsize);
		if(t_data.game.exiting) {
			break;
		}
	}

	printf("Connection closed for %d\n",t_data.sock);

breakout:;
	telnet_free(t_data.telnet);
	close(sockfd);
}
