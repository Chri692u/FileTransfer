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

extern char buffer[MESSAGE_SIZE];
extern char token[MESSAGE_SIZE];
extern int lh; /*Look ahead variable*/

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

int sendMessage(int sockd, Message msg) {
	int success;
	char type[MESSAGE_SIZE];

	sprintf(type, "%d",msg.type);
	strncat(type, msg.Message, MESSAGE_SIZE);
	success = write(sockd, type, MESSAGE_SIZE);

	checkSuccess(success);

	return 1;
}

Message awaitMessage(int sockd) {

	int success;
	char data[MESSAGE_SIZE];
	char* type;
	Message msg;

	success = read(sockd, data, sizeof(data));
	checkSuccess(success);

	msg.type = strtol(data, &type,10);
	strncpy(msg.Message,type,sizeof(msg.Message));

	memset(&data, 0, sizeof(data));
	return msg;
}

int sendReply(int sockd, char* reply){
	int success;
	success = write(sockd, reply, sizeof(reply));
	checkSuccess(success);
	return 1;
}

int awaitReply(int sockd, char* reply){
	int success;
	char data[MESSAGE_SIZE];

	success = read(sockd, reply, MESSAGE_SIZE);
	memset(&data, 0, sizeof(data));

	checkSuccess(success);

	return 1;
}