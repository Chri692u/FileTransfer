
all: client server

client: client.c
	gcc client.c -o client.out -ansi -pedantic -g inclusions/sockets.c inclusions/cli.c
server: server.c
	gcc server.c -o server.out -ansi -pedantic -g inclusions/sockets.c inclusions/cli.c


# # Src
# SOURCES=client.c server.c inclusions/cli.c inclusions/sockets.c

# #headers
# HEADERS=inclusions/cli.h inclusions/sockets.h

# #Output
# OBJECTS=$(SOURCES:.c=.o)

# #Binary
# PROG=ft

# #Flags
# CFLAGS=-ansi -pedantic -Wall -Wextra -Werror -g

# #Clean
# clean:
# 	@rm -rf $(OBJECTS) $(PROG)
# 	@echo [RM] $(OBJECTS) $(PROG)
