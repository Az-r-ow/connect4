make: connect4.c
	clang -lcurses connect4.c -o connect4

test: test.c
	clang -lcurses test.c -o test
