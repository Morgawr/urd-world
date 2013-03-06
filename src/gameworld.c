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

void handle_telnet(telnet_t *telnet, telnet_event_t *event, void *data)
{
	printf("We got dat telnet event yo\n");
}


/* This is the main entry point in our world. Nothing is shared with the 
 * actual connection server so we don't need to ever go back there.
 */
void urd_main(int sockfd) 
{
	int ret;
	int bufsize = 4096;
	unsigned char buffer[bufsize];
	telnet_t *telnet = telnet_init(opts, handle_telnet, 0, &sockfd);
	while(1) {
		ret = recv(sockfd, buffer, bufsize, 0);
		if (ret < 0) {
			fprintf(stderr,"Error when receiving message\n");
			perror("recv");
		}
		telnet_recv(telnet, buffer, ret);

		memset(buffer, 0, bufsize);
	}

	telnet_free(telnet);
	close(sockfd);
}
