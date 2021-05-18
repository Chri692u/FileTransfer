#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include "cli.h"
#include "files.h"

int checkFile(char* fileName){
	if(access(fileName, F_OK) == 0) {
		return IsFile;
	}
	return NoFile;
}

int checkFolder(char* folderName){
	DIR* directory = opendir(folderName);
	if (directory){
		return 0;
	}
	return 1;
}

Message ls() {
	struct dirent *df; /*Directory file*/
	DIR *fn;
	char cwd[DIR_LENGTH];
	Message msg;

 	if(getcwd(cwd, sizeof(cwd)) == NULL){
		 perror("CWD ERROR");
	}
	
	fn = opendir(cwd); /*File Navigator*/
	
	if(!fn) {
		perror("Unable to read directory");
		exit(0);
	}
	while((df = readdir(fn)) != NULL){
		if( (char)df->d_name[0] == '.'){
			continue;
		}
		strncat(msg.Message, df->d_name, DIR_LENGTH);
		strncat(msg.Message, "\n", DIR_LENGTH);
	}
	printf("ls : %s", msg.Message);
	return msg;
}

Message lsf(char* folderName){
	struct dirent *df;
	DIR *fn;
	char cwd[DIR_LENGTH];
	Message msg;

	if(getcwd(cwd, sizeof(cwd)) == NULL){
		perror("CWD ERROR");
	}
	strncat(cwd, "/", DIR_LENGTH - 1);
	strncat(cwd, folderName, DIR_LENGTH -1);
	fn = opendir(cwd);
	if(!fn){
		perror("Unable to read directory");
		exit(0);
	}
	while ((df = readdir(fn)) != NULL){
		if( (char)df->d_name[0] == '.'){
			continue;
		}
		strncat(msg.Message, df->d_name, DIR_LENGTH);
		strncat(msg.Message, "\n", DIR_LENGTH);
	}
	printf("lsf : %s", msg.Message);
	return msg;
}