#define BUFFER_SIZE 2048

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
Command awaitMessage(int);