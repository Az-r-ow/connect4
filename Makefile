CC=clang
CFLAGS=-lcurses -Wall -fsanitize=address
SRCS=$(wildcard src/*.c)
OBJS=$(patsubst src/%.c, obj/%.o, $(SRCS))

all: bin/connect4v2

bin/connect4v2: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -rf bin/* obj/*