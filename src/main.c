#include <stdio.h>
#include <libtelnet.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <urd_macros.h>
#include <urd_interface.h>
#include <string.h>

void strstrip(char *str)
{
	char *begin;
	char *end;
	int newlen;

	if(str == NULL || str[0] == '\0') {
		return; /* Empty string */
	}
	for(begin = str; isspace(begin[0]); begin++);
	for(end = str+strlen(str); isspace(end[0]); end--);

	newlen = end - begin;
	memcpy(str,begin,newlen);
	str[newlen] = '\0';
}

int main(int argc, const char *argv[])
{
	int conn_fd = 0;
	int ret;
	struct sockaddr_in addr;
	unsigned int addrlen;
	int acc_fd = 0;
	int opt = 1;

	conn_fd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(conn_fd, SOL_SOCKET, SO_REUSEADDR,
			(char *)&opt, sizeof(opt));

	if (conn_fd < 0) {
		fprintf(stderr,"Failed to set up socket\n");
		perror("socket");
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(9095);
	addr.sin_addr.s_addr = INADDR_ANY;
	ret = bind(conn_fd, (struct sockaddr *)&addr, sizeof(addr));

	if (ret < 0) {
		fprintf(stderr, "Failed to bind address and port\n");
		perror("bind");
	}
	
	ret = listen(conn_fd, 5);
	if (ret < 0) {
		fprintf(stderr, "Failed to listen\n");
		perror("listen");
	}

	while(1) {
		addrlen = sizeof(addr);
		acc_fd = accept(conn_fd, (struct sockaddr *)&addr, &addrlen);
		if (acc_fd < 0) {
			fprintf(stderr, "Failed to accept connection\n");
			perror("accept");
		}
		setsockopt(acc_fd, SOL_SOCKET, SO_KEEPALIVE,
				(char *)&opt, sizeof(opt));
		
		if(fork() == 0) {
			urd_main(acc_fd);
			break;
		}
	}
	close(conn_fd);

	return 0;
}
