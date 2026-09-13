# Makefile for blockchain/v0.1

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -pedantic
IFLAGS  = -I. -I../../crypto
LFLAGS  = -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

LIB     = libhblk_blockchain_v0.1.a

SRCS    = blockchain_create.c \
          block_create.c \
          block_destroy.c \
          blockchain_destroy.c \
          block_hash.c \
          blockchain_serialize.c \
          blockchain_deserialize.c \
          block_is_valid.c

OBJS    = $(SRCS:.c=.o)

PROVIDED = provided/_blockchain_print.c \
           provided/_blockchain_destroy.c \
           provided/_endianness.c \
           provided/_genesis.c

all: $(LIB)

$(LIB): $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

# --- Test targets ---

blockchain_create-test: blockchain_create.c \
    test/blockchain_create-main.c \
    $(PROVIDED)
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $^ $(LFLAGS)

blockchain_destroy-test: blockchain_create.c \
    block_create.c block_destroy.c blockchain_destroy.c \
    test/blockchain_destroy-main.c \
    $(PROVIDED)
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $^ $(LFLAGS)

block_create-test: blockchain_create.c block_create.c \
    test/block_create-main.c \
    $(PROVIDED)
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $^ $(LFLAGS)

block_destroy-test: blockchain_create.c block_create.c block_destroy.c \
    test/block_destroy-main.c \
    $(PROVIDED)
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $^ $(LFLAGS)

block_hash-test: blockchain_create.c block_create.c block_destroy.c \
    block_hash.c \
    test/block_hash-main.c \
    $(PROVIDED)
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $^ $(LFLAGS)

block_is_valid-test: blockchain_create.c block_create.c block_destroy.c \
    block_hash.c block_is_valid.c \
    test/block_is_valid-main.c \
    $(PROVIDED)
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $^ $(LFLAGS)

blockchain_serialize-test: blockchain_create.c block_create.c block_destroy.c \
    block_hash.c blockchain_serialize.c \
    test/blockchain_serialize-main.c \
    $(PROVIDED)
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $^ $(LFLAGS)

blockchain_deserialize-test: blockchain_create.c block_create.c block_destroy.c \
    block_hash.c blockchain_serialize.c blockchain_deserialize.c \
    blockchain_destroy.c \
    test/blockchain_deserialize-main.c \
    $(PROVIDED)
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $^ $(LFLAGS)

clean:
	$(RM) $(OBJS) $(LIB) \
	    blockchain_create-test \
	    blockchain_destroy-test \
	    block_create-test \
	    block_destroy-test \
	    block_hash-test \
	    block_is_valid-test \
	    blockchain_serialize-test \
	    blockchain_deserialize-test

.PHONY: all clean
