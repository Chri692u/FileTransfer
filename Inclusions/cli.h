#define MESSAGE_SIZE 1028

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
	SendFile
};

/*Prototypes*/
void readNextLine();
void skipWhiteSpace();
void getNextToken();
Message parseMessage();
int sendMessage(int, Message);
Message awaitMessage(int);
int sendReply(int sockd, char* reply);
int awaitReply(int sockd, char* reply);