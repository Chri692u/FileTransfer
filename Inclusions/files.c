#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
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

Message ls(char* dir, int hidden, int list) {
	struct dirent *d;
	DIR *fn = opendir(dir);
	if(!fn) {
		if (errno = ENOENT){
			perror("Directory does not exist:");
		}
		
	}
}

void lsf(char* folderName){

}