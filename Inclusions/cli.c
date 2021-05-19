#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include "cli.h"
#include "sockets.h"

extern char buffer[MESSAGE_SIZE];
extern char token[MESSAGE_SIZE];
extern int lh; /*Look ahead variable*/

/*Return type Message*/
Message awaitMessage(int sockd) {
	int success;
	char data[MESSAGE_SIZE];
	char* type;
	Message msg;


	success = recv(sockd, data, sizeof(data),0);
	checkSuccess(success);

	msg.type = strtol(data, &type,10);

	strncpy(msg.Message,type,sizeof(msg.Message));
	memset(&data, 0, sizeof(data));
	return msg;
}

Message parseMessage() {
	Message msg;
	getNextToken();
	if (strncmp(token, "quit", MESSAGE_SIZE) == 0) {
		msg.type = Quit;
		return msg; /*Return instantly*/
	} else if (strncmp(token, "clear", MESSAGE_SIZE) == 0){
		msg.type = Clear;
		return msg;
	}  else if (strncmp(token, "ls", MESSAGE_SIZE) == 0){
		msg.type = Ls;
		return msg; /*ls can take 0 or 1 arguments*/
	} else if (strncmp(token, "help", MESSAGE_SIZE) == 0){
		msg.type = Help;
		return msg; /*No need for getting next token*/
	} else if (strncmp(token, "send", MESSAGE_SIZE) == 0){
		msg.type = Send;
	} else if (strncmp(token, "req", MESSAGE_SIZE) == 0){
		msg.type = Request;
	} else if (strncmp(token, "lsf", MESSAGE_SIZE) == 0){
		msg.type = LsFolder; /*ls can take 0 or 1 arguments*/
		getNextToken();
		strncpy(msg.Message, token, sizeof(token));
		return msg;
	}
	getNextToken();
	strncpy(msg.Message, token, sizeof(token));

	return msg;
}

/*Return type int*/
int awaitReply(int sockd, char* reply){
	int success;

	success = recv(sockd, reply, sizeof(reply),0);
	checkSuccess(success);

	return 1;
}

int sendMessage(int sockd, Message msg) {
	int success;
	char type[MESSAGE_SIZE];

	sprintf(type, "%d",msg.type);
	strncat(type, msg.Message, MESSAGE_SIZE);

	success = send(sockd, type, sizeof(type),0);
	checkSuccess(success);

	return 1;
}


int sendReply(int sockd, char* reply){
	int success;
	success = send(sockd, reply, sizeof(reply),0);
	checkSuccess(success);

	return 1;
}

/*Return type void*/
void getNextToken(){
	int i = 0;
	char temp;

	if (buffer[lh] == '\0'){
		readNextLine();
	}
	skipWhiteSpace();

	temp = buffer[lh];
	while (temp != ' ' && temp != '\0'){
		token[i] = buffer[lh];
		i++;
		lh++;
		temp = buffer[lh];
	}
	token[i] = '\0';
}

void prettyPrintHelp(){
	system("clear");
	printf("Help commands:\n");
	printf("All commands follow this structure: Command Message\n");
	printf("Quit will close the client\n");
	printf("Clear will clear screen\n");
	printf("ls will list all contents of the server\n");
	printf("Send 'filename' will send a file to the server\n");
	printf("Req 'filename' will request a file from the server\n");
	printf("lsf will list the content of a folder on the server\n");
}

void readNextLine(){
	int i = 0;
	lh = 0;
	fgets(buffer, MESSAGE_SIZE, stdin);
	while (lh) {
		if (buffer[0] == '\n') {
			fgets(buffer, MESSAGE_SIZE, stdin);
		}
		else {
			break;
		}
	}
	while (buffer[i] != '\n') {
		i++;
	}
	buffer[i] = '\0';
}

void skipWhiteSpace() {
	while (buffer[lh] == ' ') {
		lh++;
	}
}

void sendFile(FILE *fp, int socket){
	char data[BUFFER_SIZE] = {0};

	while(fgets(data,BUFFER_SIZE,fp) != NULL){
		if(send(socket,data,sizeof(data),0) == -1){
			perror("Error sending data \n");
			exit(0);
		}
		memset(data, BUFFER_SIZE, sizeof(data));
	}
}

void sendFileCommand(int sockd, Message msg){
	FILE *fp;
	int serverReply;
	char reply[MESSAGE_SIZE], *errType;

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
}

void writeFile(int socket, Message msg){
	int n;
	FILE *fp;
	char chunk[BUFFER_SIZE];

	fp = fopen(msg.Message, "w");
	if(fp == NULL){
		perror("Error writing file \n");
		exit(0);
	}

	while(1){
		n = recv(socket, chunk, BUFFER_SIZE, 0);
		if(n <= 0){
			break;
		}
		fprintf(fp, "%s", chunk);
		memset(chunk,BUFFER_SIZE,sizeof(chunk));
	}
	return;
}

void LsCommand(int sockd, Message msg) {
	Message list;

	sendMessage(sockd, msg);
	list = awaitMessage(sockd);
	printf("\tList of files:\n%s", list.Message);
}

void LsfCommand(int sockd, Message msg) {
	Message list;
	sendMessage(sockd, msg);
	list = awaitMessage(sockd);
	printf("List of files in folder: %s\n%s",msg.Message, list.Message);
}