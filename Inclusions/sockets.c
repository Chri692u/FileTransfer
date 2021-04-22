void checkDescriptor(int sockd) {
	if (sockd == -1) {
		perror("Failed to create socket");
		exit(0);
	}
}

void checkConnection(int connection){
	if(connection != 0){
		perror("Failed to connect to server");
		exit(0);
	}
}

void checkBinding(int sockbind){
	if(sockbind != 0){
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