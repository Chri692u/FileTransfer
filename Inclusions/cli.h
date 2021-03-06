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
	LsFolder,
	Help,
	Send,
	Request,
	IsFile,
	NoFile
};

/*Prototypes*/

/*Return type Message*/
Message awaitMessage(int);
Message parseMessage();

/*Return type int*/
int awaitReply(int, char*);
int sendMessage(int, Message);
int sendReply(int, char*);

/*Return type void*/
void getNextToken();
/*void getFile(int, Message); - NOT WORKING ATM*/
void prettyPrintHelp();
void LsCommand(int, Message);
void LsfCommand(int, Message);
void readNextLine();
void requestFileCommand(int, Message);
void skipWhiteSpace();
void sendFile(FILE*, int);
void sendFileCommand(int, Message);
void writeFile(int, Message);
