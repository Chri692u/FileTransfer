#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include "inclusions/cli.h"
#include "Inclusions/sockets.h"

int main(){
	int running = 1, sockd, sockbind, connection, len;
	struct sockaddr_in server, client;

	do {
		sockd = socket(AF_INET, SOCK_STREAM, 0);
		checkDescriptor(sockd);

		/*Zero memory*/
		memset(&server, 0, sizeof(server));

		/*Asign server struct*/
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = htonl(INADDR_ANY);
		server.sin_port = htons(8080);

		/*Binding the socket*/
		sockbind = bind(sockd, (struct sockaddr*)&server, sizeof(server));
		checkBinding(sockbind);

		/*Listening*/
		if((listen(sockd, 5)) != 0){
			perror("Failed to listen");
			exit(0);
		}
		printf("Server listening...\n");

		/*Accept Data*/
		len = sizeof(client);
		connection = accept(sockd, (struct sock_addr*)&client, &len);
		checkAccept(connection);

	} while (running);
	/*Finishing up...*/
	close(sockd);
}