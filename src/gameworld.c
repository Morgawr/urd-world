#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <libtelnet.h>
#include <urd_macros.h>
#include <urd_interface.h>
#include <string.h>

static const telnet_telopt_t opts[] = {
	{ TELNET_TELOPT_ECHO,      TELNET_WILL, TELNET_DONT },
	{ TELNET_TELOPT_TTYPE,     TELNET_WILL, TELNET_DONT },
	{ TELNET_TELOPT_COMPRESS2, TELNET_WONT, TELNET_DONT },
	{ TELNET_TELOPT_ZMP,       TELNET_WONT, TELNET_DONT },
	{ TELNET_TELOPT_MSSP,      TELNET_WONT, TELNET_DONT },
	{ TELNET_TELOPT_BINARY,    TELNET_WONT, TELNET_DONT },
	{ TELNET_TELOPT_NAWS,      TELNET_WONT, TELNET_DONT },
	{ -1, 0, 0 }
};

struct telnet_data {
	int sock;
	telnet_t *telnet;
};

void handle_telnet(telnet_t *telnet, telnet_event_t *event, void *data)
{
	switch(event->type) {
		case TELNET_EV_DATA:
			printf("Got TELNET_EV_DATA\n");
			break;
		case TELNET_EV_SEND:
			printf("Got TELNET_EV_SEND\n");
			break;
		case TELNET_EV_IAC:
			printf("Got TELNET_EV_IAC\n");
			break;
		case TELNET_EV_WILL:
			printf("Got TELNET_EV_WILL\n");
			break;
		case TELNET_EV_WONT:
			printf("Got TELNET_EV_WONT\n");
			break;
		case TELNET_EV_DO:
			printf("Got TELNET_EV_DO\n");
			break;
		case TELNET_EV_DONT:
			printf("Got TELNET_EV_DONT\n");
			break;
		case TELNET_EV_SUBNEGOTIATION:
			printf("Got TELNET_EV_SUBNEGOTIATION\n");
			break;
		case TELNET_EV_COMPRESS:
			printf("Got TELNET_EV_COMPRESS\n");
			break;
		case TELNET_EV_ZMP:
			printf("Got TELNET_EV_ZMP\n");
			break;
		case TELNET_EV_TTYPE:
			printf("Got TELNET_EV_TTYPE\n");
			break;
		case TELNET_EV_ENVIRON:
			printf("Got TELNET_EV_ENVIRON\n");
			break;
		case TELNET_EV_MSSP:
			printf("Got TELNET_EV_MSSP\n");
			break;
		case TELNET_EV_WARNING:
			printf("Got TELNET_EV_WARNING\n");
			break;
		case TELNET_EV_ERROR:
			printf("Got TELNET_EV_ERROR\n");
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
	}

breakout:;
	telnet_free(t_data.telnet);
	close(sockfd);
}
