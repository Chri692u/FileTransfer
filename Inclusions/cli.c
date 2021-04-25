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

Command parseCommand() {
	Command command;
	getNextToken();
	if (strncmp(token, "quit", MESSAGE_SIZE) == 0) {
		command.type = Quit;
		return command; /*Return instantly*/
	} else if (strncmp(token, "clear", MESSAGE_SIZE) == 0){
		command.type = Clear;
		return command;
	}  else if (strncmp(token, "ls", MESSAGE_SIZE) == 0){
		command.type = Ls;
		return command; /*ls can take 0 or 1 arguments*/
	} else if (strncmp(token, "help", MESSAGE_SIZE) == 0){
		command.type = Help;
		return command; /*No need for getting next token*/
	} else if (strncmp(token, "send", MESSAGE_SIZE) == 0){
		command.type = Send;
	} else if (strncmp(token, "req", MESSAGE_SIZE) == 0){
		command.type = Request;
	} else if (strncmp(token, "lsf", MESSAGE_SIZE) == 0){
		command.type = LsFolder; /*ls can take 0 or 1 arguments*/
		getNextToken();
		strncpy(command.commandMsg, token, sizeof(token));
		return command; 
	}
	getNextToken();
	strncpy(command.commandMsg, token, sizeof(token));
	return command;
}

int sendMessage(int sockd, Command msg) {
	int success;
	char type[MESSAGE_SIZE];
	type[0] = msg.type;

	success = write(sockd, type, 1);
	success = write(sockd, msg.commandMsg, strnlen(msg.commandMsg, MESSAGE_SIZE));
	if (success == -1){
		perror("Failed sending the file");
		exit(0);
	}
	memset(&msg, 0, sizeof(msg));
	return 1;

}

Command awaitMessage(int sockd) {
	int success, type;
	char data[MESSAGE_SIZE];
	Command command;
	memset(&command, 0, sizeof(command));
	success = read(sockd, data, strnlen(data, MESSAGE_SIZE));
	command.type = (int) data[0];
	success = read(sockd, command.commandMsg, MESSAGE_SIZE);

	if (success == -1){
		perror("Failed to receive file");
		exit(0);
	}
	/*data[0] = '\n';*/
	memset(&data, 0, sizeof(data));
	return command;
}