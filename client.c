#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>;
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
	struct sockaddr_in server;

	printf("Command Line Interface is running.\n> ");
	do {
		sockd = socket(AF_INET, SOCK_STREAM, 0);
		checkDescriptor(sockd);
		
		/*Zero memory*/
		memset(&server, 0, sizeof(server));

		server.sin_addr.s_addr = inet_addr(serverIP);
		server.sin_family = AF_INET;
		server.sin_port = htons(8080);

		connection = connect(sockd, (struct sock_addr*)&server, sizeof(server));
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
	} while (running);
	
	exit(1);
}
