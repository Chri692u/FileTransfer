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

	int running = 1, send, sockd, connection;
	Message  msg;
	char* serverIP = "127.0.0.1";
	struct sockaddr_in serverAdress;
	struct hostent *server;

	printf("Command Line Interface is running.\n> ");

	do {

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
			break;
		case Clear:
			system("clear");
			break;
		case Ls:
			printf("\tList of files:\n");
			send = sendMessage(sockd, msg);
			break;
		case Help:
			printf("\tHelp :D\n");
			break;
		case Send:
			send = sendMessage(sockd, msg);
			if(send){
				printf("\tSuccessfully sent file: %s to server\n", msg.Message);
			}
			break;
		case Request:
			send = sendMessage(sockd, msg);
			printf("\tRequesting file: %s from server\n", msg.Message);
			break;
		case LsFolder:
			send = sendMessage(sockd, msg);
			printf("List of files in folder: %s\n", msg.Message);
			break;
		default:
			printf("\tYou shouldnt be here\n");
		}
		close(sockd);
	} while(running);
	exit(1);
}
