#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <urd_macros.h>
#include <urd_interface.h>
#include <string.h>


/* This is the main entry point in our world. Nothing is shared with the 
 * actual connection server so we don't need to ever go back there.
 */
void urd_main(int sockfd) 
{
	int ret;
	int bufsize = 4096;
	char buffer[bufsize];
	while(1) {
		ret = recv(sockfd, buffer, bufsize, 0);
		if (ret < 0) {
			fprintf(stderr,"Error when receiving message\n");
			perror("recv");
		}

		strstrip(buffer);

		if(strncmp(buffer,"exit",strlen("exit")) == 0)
			break;

		DBG(("#%d - Received: %s", sockfd, buffer));
		memset(buffer, 0, bufsize);
	}

	close(sockfd);
}
