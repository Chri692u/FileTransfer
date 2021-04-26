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
	Message Message;
	getNextToken();
	if (strncmp(token, "quit", MESSAGE_SIZE) == 0) {
		Message.type = Quit;
		return Message; /*Return instantly*/
	} else if (strncmp(token, "clear", MESSAGE_SIZE) == 0){
		Message.type = Clear;
		return Message;
	}  else if (strncmp(token, "ls", MESSAGE_SIZE) == 0){
		Message.type = Ls;
		return Message; /*ls can take 0 or 1 arguments*/
	} else if (strncmp(token, "help", MESSAGE_SIZE) == 0){
		Message.type = Help;
		return Message; /*No need for getting next token*/
	} else if (strncmp(token, "send", MESSAGE_SIZE) == 0){
		Message.type = Send;
	} else if (strncmp(token, "req", MESSAGE_SIZE) == 0){
		Message.type = Request;
	} else if (strncmp(token, "lsf", MESSAGE_SIZE) == 0){
		Message.type = LsFolder; /*ls can take 0 or 1 arguments*/
		getNextToken();
		strncpy(Message.Message, token, sizeof(token));
		return Message; 
	}
	getNextToken();
	strncpy(Message.Message, token, sizeof(token));
	return Message;
}

int sendMessage(int sockd, Message msg) {
	int success;
	char type[MESSAGE_SIZE];
	sprintf(type, "%d",msg.type);
	success = write(sockd, type, sizeof(type));
	success = write(sockd, msg.Message, strnlen(msg.Message, MESSAGE_SIZE));
	checkSuccess(success);

	return 1;
}

Message awaitMessage(int sockd) {
	int success;
	char data[MESSAGE_SIZE];
	char* type;
	Message msg;

	success = read(sockd, data, strnlen(data, MESSAGE_SIZE));
	msg.type = strtol(data, &type, 10);
	success = read(sockd, msg.Message, MESSAGE_SIZE);
	checkSuccess(success);
	memset(&data, 0, sizeof(data));
	return msg;
}

int sendReply(int sockd, char* reply){
	int success;

	success = write(sockd, reply, strnlen(reply, MESSAGE_SIZE));
	checkSuccess(success);

	return 1;
}

int awaitReply(int sockd, char* reply){
	int success;
	memset(&reply, 0, sizeof(reply));

	success = read(sockd, reply, strnlen(reply, MESSAGE_SIZE));
	checkSuccess(success);

	return 1;
}