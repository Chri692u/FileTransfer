#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include "inclusions/cli.h"
#include "Inclusions/sockets.h"
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

/*Globals*/
char token[MESSAGE_SIZE];
char buffer[MESSAGE_SIZE];
int lh;

int main(){
	int running = 1, sockd, sockbind, len, newsockd;
	struct sockaddr_in server, client;
	socklen_t sockSize;
	Command msg;


	do {
		sockd = socket(AF_INET, SOCK_STREAM, 0);
		checkDescriptor(sockd);

		/*Zero memory*/
		memset((char *)&server, 0, sizeof(server));

		/*Asign server struct*/
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_port = htons(8080);
		
		/*Binding the socket*/
		sockbind = bind(sockd, (struct sockaddr* )&server, sizeof(server));
		checkBinding(sockbind);
		/*Listening*/
		listen(sockd, 10);
		printf("Server listening...\n");

		/*Accept Data*/
		sockSize = sizeof(client);

		newsockd = accept(sockd, (struct sockaddr* )&client, &sockSize);
		checkAccept(newsockd);

		msg = awaitMessage(newsockd);

		switch (msg.type)
		{
		case Ls:
			printf("\tList of files:\n");
			break;
		case Send:
			printf("\tIncoming file: %s\n", msg.commandMsg);
			break;
		case Request:
			printf("\tClient requested file: %s\n", msg.commandMsg);
			break;
		case LsFolder:
			printf("\tList of files in folder: %s\n", msg.commandMsg);
			break;
		}
		close(sockd);
	} while (running);
	/*Finishing up...*/
	close(sockd);
}