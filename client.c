#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "inclusions/cli.h"
#include "Inclusions/sockets.h"

/*Globals*/
char token[MESSAGE_SIZE];
char buffer[MESSAGE_SIZE];
int lh;

int main(){

	int running, serverReply, sockd, connection;
	Message  msg;
	Message list;
	char reply[MESSAGE_SIZE];
	char* serverIP = "127.0.0.1";
	char* errType;
	struct sockaddr_in serverAdress;
	struct hostent *server;
	FILE *fp;
	printf("Command Line Interface is running.\n> ");

	/*do {*/

		sockd = socket(AF_INET, SOCK_STREAM, 0);
		checkDescriptor(sockd);

		server = gethostbyname(serverIP);
		if(server == NULL){
			perror("Failed, could not find host");
			exit(0);
		}
		serverAdress.sin_family = AF_INET;
		serverAdress.sin_addr.s_addr = htonl(INADDR_ANY);
		serverAdress.sin_port = htons(8080);


		/*Zero memory*/
		memset((char *)&server, 0, sizeof(server));

		connection = connect(sockd, (struct sockaddr* )&serverAdress, sizeof(serverAdress));
		checkConnection(connection);

		/*Message Line Interface*/
		readNextLine();
		msg = parseMessage();


		switch (msg.type){
		case Quit:
			printf("\tQuiting...\n");
			running = 0;
			printf("haha %d", running);
			break;
		case Clear:
			system("clear");
			break;
		case Ls:

			sendMessage(sockd, msg);
			list = awaitMessage(sockd);
			printf("\tList of files:\n%s", list.Message);
			/*
			if (send){
				serverReply = awaitReply(sockd, reply);
				checkSend(serverReply);
				printf("%s", reply);
			}
			*/
			break;
		case Help:
			printf("\tHelp :D\n");
			break;
		case Send:
			sendMessage(sockd, msg);
			serverReply = awaitReply(sockd, reply);
			checkSend(serverReply);

			if(strtol(reply, &errType,10) == IsFile){
				printf("File already exists on server\n");
				exit(0);
			}


			fp = fopen(msg.Message, "r");

			if(fp == NULL){
				perror("Error reading file");
				exit(0);
			}

			sendFile(fp,sockd);

			break;
		case Request:
			printf("\tRequesting file: %s from server\n", msg.Message);
			sendMessage(sockd, msg);
			serverReply = awaitReply(sockd, reply);
			checkSend(serverReply);

			if(strtol(reply, &errType,10) == NoFile){
				printf("File does not exist on server\n");
				exit(0);
			}

			writeFile(sockd, msg);

			break;
		case LsFolder:
			sendMessage(sockd, msg);
			list = awaitMessage(sockd);
			printf("List of files in folder: %s\n%s",msg.Message, list.Message);
			break;
		default:
			printf("\tYou shouldnt be here\n");
		}
		close(sockd);
	/*} while(running);*/
	exit(1);
}
