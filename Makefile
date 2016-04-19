# Kyong Lee

# A Makefile for project 2
# Comments start with a pound sign

# Macros
CLIENT_SRCS=  Project4Client.c TCPClientUtility.c
SERVER_SRCS=  Project4Server.c HandleProj4Client.c
SRCS=DieWithError.c Database.c ByteToBinary.c ReceiveResponse.c
HDRS=NetworkHeader.h WhoHeader.h
CFLAGS= -Wall
LFLAGS= 
CC= gcc

#All commands must start with a tab (not spaces)
#Some macros are built in.  $@ is the name of the 
#target being built

all: clean Project4Server Project4Client

Project4Server: $(SERVER_SRCS) $(SRCS) $(HDRS)
	rm -f $@
	$(CC) $(CFLAGS) -o $@ $(SERVER_SRCS) $(SRCS) $(LFLAGS)

Project4Client: $(CLIENT_SRCS) $(SRCS) $(HDRS)
	rm -f $@
	$(CC) $(CFLAGS) -o $@ $(CLIENT_SRCS) $(SRCS) $(LFLAGS)


clean:
	-rm -f Project4Server Project4Client *.o
