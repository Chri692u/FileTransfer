#include <unistd.h>
#include <dirent.h>
#include "cli.h"
#include "files.h"

int checkFile(char* fileName){
	if(access(fileName, F_OK) == 0) {
		return 0;
	}
	return 1;
}

int checkFolder(char* folderName){
	DIR* directory = opendir(folderName);
	if (directory){
		return 0;
	}
	return 0;
}