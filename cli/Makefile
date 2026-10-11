CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
INC = -I../blockchain/v0.3 -I../crypto -I../blockchain/v0.3/provided
LIBS = -L../blockchain/v0.3 -L../crypto -L../blockchain/v0.3/provided \
	-lhblk_blockchain -lhblk_crypto -lllist_linux_x86_64 -lssl -lcrypto -pthread

cli: main.c
	$(CC) $(CFLAGS) $(INC) main.c -o cli $(LIBS)

clean:
	rm -f cli
