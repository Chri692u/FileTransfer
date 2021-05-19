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

	Message  msg;

	struct sockaddr_in serverAdress;
	struct hostent *server;

	int sockd, connection;
	char *serverIP = "127.0.0.1";

	printf("Command Line Interface is running.\n> ");

	/*do {*/
		/*Creaing socket*/
		sockd = socket(AF_INET, SOCK_STREAM, 0);
		checkDescriptor(sockd);

		/*Checking host connection*/
		server = gethostbyname(serverIP);
		if(server == NULL){
			perror("Failed, could not find host");
			exit(0);
		}

		/*Asign client struct*/
		serverAdress.sin_family = AF_INET;
		serverAdress.sin_addr.s_addr = htonl(INADDR_ANY);
		serverAdress.sin_port = htons(8080);

		/*Zero memory*/
		memset((char *)&server, 0, sizeof(server));

		/*Connecting to server*/
		connection = connect(sockd, (struct sockaddr* )&serverAdress, sizeof(serverAdress));
		checkConnection(connection);

		/*Message Line Interface*/
		readNextLine();
		msg = parseMessage();

		switch (msg.type){
		case Quit:
			printf("\tQuiting...\n");
			/*running = 0;*/
			break;
		case Clear:
			system("clear");
			break;
		case Ls:
			LsCommand(sockd, msg);
			break;
		case LsFolder:
			LsfCommand(sockd, msg);
			break;
		case Help:
			prettyPrintHelp();
			break;
		case Send:
			sendFileCommand(sockd, msg);
			break;
		case Request:
			requestFileCommand(sockd, msg);
			break;
		default:
			printf("\tYou shouldnt be here\n");
		}
		close(sockd);
	/*} while(running);*/
	exit(1);
}
