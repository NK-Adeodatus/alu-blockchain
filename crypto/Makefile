CC       := gcc
CFLAGS   := -Wall -Wextra -Werror -pedantic -I.
LDFLAGS  := -lssl -lcrypto
AR       := ar
ARFLAGS  := rcs

LIB      := libhblk_crypto.a

# All object files that form the static library
OBJ      := sha256.o \
             ec_create.o \
             ec_from_pub.o \
             ec_load.o \
             ec_save.o \
             ec_sign.o \
             ec_to_pub.o \
             ec_verify.o

.PHONY: all clean fclean re sha256-test ec_create-test ec_to_pub-test ec_from_pub-test ec_save-test

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ── Task 0: sha256 ────────────────────────────────────────────────────────────
sha256-test: test/sha256-main.c provided/_print_hex_buffer.c sha256.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# ── Task 1: ec_create ─────────────────────────────────────────────────────────
ec_create-test: test/ec_create-main.c ec_create.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# ── Task 2: ec_to_pub ─────────────────────────────────────────────────────────
ec_to_pub-test: test/ec_to_pub-main.c provided/_print_hex_buffer.c ec_to_pub.c ec_create.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# ── Task 3: ec_from_pub ────────────────────────────────────────────────────────
ec_from_pub-test: test/ec_from_pub-main.c provided/_print_hex_buffer.c ec_from_pub.c ec_to_pub.c ec_create.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# ── Task 4: ec_save ──────────────────────────────────────────────────────────
ec_save-test: test/ec_save-main.c provided/_print_hex_buffer.c ec_save.c ec_to_pub.c ec_create.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# ── Cleanup ───────────────────────────────────────────────────────────────────
clean:
	$(RM) $(OBJ) sha256-test

fclean: clean
	$(RM) $(LIB)

re: fclean all
