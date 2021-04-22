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
char token[BUFFER_SIZE];
char buffer[BUFFER_SIZE];
int lh;

int main(){

	int running = 1, send, sockd, connection;
	Command command;
	char* serverIP = "127.0.0.1";
	struct sockaddr_in serverAdress;
	struct hostent *server;

	printf("Command Line Interface is running.\n> ");

	// do {

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

		/*Command Line Interface*/
		readNextLine();
		command = parseCommand();


		switch (command.type){
		case 0:
			printf("\tQuiting...\n");
			running = 0;
			break;
		case 1:
			system("clear");
			break;
		case 2:
			printf("\tList of files:\n");
			break;
		case 3:
			printf("\tHelp :D\n");
			break;
		case 4:
			send = sendMessage(sockd, command);
			if(send){
				printf("\tSuccessfully sent file: %s to server\n", command.commandMsg);
			}
			break;
		case 5:
			printf("\tRequesting file: %s from server\n", command.commandMsg);
			break;
		case 6:
			printf("List of files in folder: %s\n", command.commandMsg);
			break;
		default:
			printf("\tYou shouldnt be here\n");
		}
		close(sockd);
	// } while (running);

	exit(1);
}
