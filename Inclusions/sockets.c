#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "cli.h"

void checkDescriptor(int sockd) {
	if (sockd == -1) {
		perror("Failed to create socket");
		exit(0);
	}
}

void checkConnection(int connection){
	if(connection < 0){
		perror("Failed to connect to server");
		exit(0);
	}
}

void checkBinding(int sockbind){
	if(sockbind < 0){
		perror("Failed to bind socket");
		exit(0);
	}	
}

void checkAccept(int connection){
	if(connection < 0){
		perror("Server failed to accept data");
		exit(0);
	}
}

void checkSend(int send){
	if(send == -1){
		perror("Failed to send data");
		exit(0);
	}
}

void checkReply(int reply){
	if(reply == -1){
		perror("Failed to receive reply");
		exit(0);
	}
}
