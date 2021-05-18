#define MESSAGE_SIZE 1028
#define BUFFER_SIZE 2048

typedef struct Message {
	int type;
	char Message[MESSAGE_SIZE];
} Message;

enum MessageType{
	Quit,
	Clear,
	Ls,
	Help,
	Send,
	Request,
	LsFolder,
	IsFile,
	NoFile,
	Recieved
};

/*Prototypes*/
void readNextLine();
void skipWhiteSpace();
void getNextToken();
void sendFile(FILE *fp, int socket);
void writeFile(int socket, Message msg);
Message parseMessage();
int sendMessage(int, Message);
Message awaitMessage(int);
int sendReply(int sockd, char* reply);
int awaitReply(int sockd, char* reply);
void prettyPrintHelp();