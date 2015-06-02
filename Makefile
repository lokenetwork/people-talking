.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=p_t_p_server p_t_p_client
all:$(BIN)
%.o:%.c
				$(CC) $(CFLAGS) -c $< -0 $@
clean:
				rm -f *.o $(BIN)


