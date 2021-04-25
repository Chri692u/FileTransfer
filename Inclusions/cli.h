#define MESSAGE_SIZE 128

typedef struct Command {
	int type;
	char commandMsg[MESSAGE_SIZE];
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
int sendMessage(int, Command);
Command awaitMessage(int);