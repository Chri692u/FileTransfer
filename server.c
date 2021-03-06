#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include "Inclusions/cli.h"
#include "Inclusions/sockets.h"
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <dirent.h>
#include "Inclusions/files.h"

/*Globals*/
char token[MESSAGE_SIZE];
char buffer[MESSAGE_SIZE];
int lh;

int main(){

	Message msg, reply;

	FILE *fp;

	socklen_t sockSize;
	struct sockaddr_in server, client;

	int running = 1, sockd, sockbind, len, newsockd, success;


	/*do {*/
		/*Creaing socket*/
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

		/*Accept connection*/
		sockSize = sizeof(client);
		newsockd = accept(sockd, (struct sockaddr* )&client, &sockSize);
		checkAccept(newsockd);

		/*Accept incoming data*/
		msg = awaitMessage(newsockd);
		switch (msg.type){
		case Ls:
			printf("List of files command recieved:\n");
			reply = ls();
			success = send(sockd, reply.Message, sizeof(reply.Message), 0);
			checkSend(success);
			break;
		case LsFolder:
			if (!checkFolder(msg.Message)){
				printf("\tDirectory OK - Sending reply\n");
				reply = lsf(msg.Message);
				success = send(newsockd, reply.Message, sizeof(reply.Message),0);
				checkSend(success);
				break;
			}
			printf("\tDirectory not OK - Sending error message\n");
			break;
		case Send:
			printf("\tIncoming file: %s\n", msg.Message);
			if (checkFile(msg.Message) != IsFile){
				sendReply(newsockd, "8");
				writeFile(newsockd, msg);

				printf("\tFile OK - Sending reply NOT WORKING\n");
				break;
			}
			printf("\tFile not OK - Sending reply\n");
			sendReply(newsockd, "7");
			break;
		case Request:
			printf("\tClient requested file: %s\n", msg.Message);
			if(checkFile(msg.Message) == IsFile){
				sendReply(newsockd, "7");
				printf("File OK - Accepting request\n");
				fp = fopen(msg.Message, "r");

				if(fp == NULL){
					perror("Error reading file");
					sendReply(newsockd, "8");
					exit(0);
				}

				sendFile(fp, newsockd);
				break;
			}
			printf("\tFile not OK - Sending reply\n");
			sendReply(newsockd, "8");

			break;
		}
		close(newsockd);
		close(sockd);
	/*} while (running);*/
	/*Finishing up...*/
	close(newsockd);
	close(sockd);
}