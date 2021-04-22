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

extern char buffer[BUFFER_SIZE];
extern char token[BUFFER_SIZE];
extern int lh; /*Look ahead variable*/

void readNextLine(){
	int i = 0;
	lh = 0;
	fgets(buffer, BUFFER_SIZE, stdin);
	while (lh) {
		if (buffer[0] == '\n') {
			fgets(buffer, BUFFER_SIZE, stdin);
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

Command parseCommand() {
	Command command;
	getNextToken();
	if (strncmp(token, "quit", BUFFER_SIZE) == 0) {
		command.type = Quit;
		return command; /*Return instantly*/
	} else if (strncmp(token, "clear", BUFFER_SIZE) == 0){
		command.type = Clear;
		return command;
	}  else if (strncmp(token, "ls", BUFFER_SIZE) == 0){
		command.type = Ls;
		return command; /*ls can take 0 or 1 arguments*/
	} else if (strncmp(token, "help", BUFFER_SIZE) == 0){
		command.type = Help;
		return command; /*No need for getting next token*/
	} else if (strncmp(token, "send", BUFFER_SIZE) == 0){
		command.type = Send;
	} else if (strncmp(token, "req", BUFFER_SIZE) == 0){
		command.type = Request;
	} else if (strncmp(token, "lsf", BUFFER_SIZE) == 0){
		command.type = LsFolder; /*ls can take 0 or 1 arguments*/
		getNextToken();
		command.commandMsg = token;
		return command; 
	}
	getNextToken();
	command.commandMsg = token;
	return command;
}

int sendMessage(int sockd, Command msg) {
	int success;

	success = write(sockd, msg.commandMsg, strnlen(msg.commandMsg,BUFFER_SIZE));
	if (success == -1){
		perror("Failed sending the file");
		exit(0);
	}
	// memset(&msg, 0, sizeof(msg));
	return 1;

}

void awaitMessage(int sockd, char *data) {
	int success;
	success = read(sockd, data, BUFFER_SIZE);

	if (success == -1){
		perror("Failed to receive file");
		exit(0);
	}

}