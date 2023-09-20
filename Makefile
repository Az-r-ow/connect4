CC=clang
CFLAGS=-lcurses -Wall 
SRCS=$(wildcard src/*.c)
OBJS=$(patsubst src/%.c, obj/%.o, $(SRCS))

all: bin/connect4

bin/connect4: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -rf bin/* obj/*