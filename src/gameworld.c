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
			break;
		case TELNET_EV_SEND:
			break;
		case TELNET_EV_IAC:
			break;
		case TELNET_EV_WILL:
			break;
		case TELNET_EV_WONT:
			break;
		case TELNET_EV_DO:
			break;
		case TELNET_EV_DONT:
			break;
		case TELNET_EV_SUBNEGOTIATION:
			break;
		case TELNET_EV_COMPRESS:
			break;
		case TELNET_EV_ZMP:
			break;
		case TELNET_EV_TTYPE:
			break;
		case TELNET_EV_ENVIRON:
			break;
		case TELNET_EV_MSSP:
			break;
		case TELNET_EV_WARNING:
			break;
		case TELNET_EV_ERROR:
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
