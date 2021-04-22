#define BUFFER_SIZE 256

typedef struct Command {
	int type;
	char* commandMsg;
} Command;

enum CommandType{
	Quit,
	Clear,
	Ls,
	Help,
	Send,
	Request,
	LsFolder
};

/*Prototypes*/
void readNextLine();
void skipWhiteSpace();
void getNextToken();
Command parseCommand();
int sendMessage(int,Command);
void awaitMessage(int, char*);