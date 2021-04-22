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
char token[BUFFER_SIZE];
char buffer[BUFFER_SIZE];
int lh;

int main(){
	int running = 1, option = 1, sockd, sockbind, len, newsockd;
	struct sockaddr_in server, client;
	char data[BUFFER_SIZE];


	// do {
		sockd = socket(AF_INET, SOCK_STREAM, 0);
		// setsockopt(sockd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
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
		len = sizeof(client);

		newsockd = accept(sockd, (struct sockaddr* )&client, &len);
		printf("%d",newsockd);
		checkAccept(newsockd);
		// prinf("%d",newsockd);

		

		awaitMessage(newsockd, data);
		printf("%s",data);
	

		// close(sockd);
		// memset(&server, 0, sizeof(server));
	// } while (running);
	/*Finishing up...*/
	close(sockd);
}