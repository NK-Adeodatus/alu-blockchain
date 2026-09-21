CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -g3
INCLUDES = -I. -Itransaction/ -I../../crypto

SRCS = blockchain_create.c block_create.c block_destroy.c \
	blockchain_destroy.c block_hash.c blockchain_serialize.c \
	blockchain_deserialize.c block_is_valid.c hash_matches_difficulty.c \
	blockchain_difficulty.c block_mine.c \
	transaction/tx_out_create.c transaction/unspent_tx_out_create.c \
	transaction/tx_in_create.c transaction/transaction_hash.c \
	transaction/tx_in_sign.c transaction/transaction_create.c \
	transaction/transaction_is_valid.c transaction/coinbase_create.c \
	transaction/coinbase_is_valid.c transaction/transaction_destroy.c \
	transaction/update_unspent.c

OBJS = $(SRCS:.c=.o)

libhblk_blockchain.a: $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f libhblk_blockchain.a

re: fclean libhblk_blockchain.a

.PHONY: clean fclean re
