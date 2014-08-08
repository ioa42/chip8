CC = clang
CFLAGS = -std=c99 -g3

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

OUT = chip8

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: clean

clean:
	$(RM) $(OBJ) $(OUT)
