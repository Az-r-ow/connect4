make: connect4.c
	clang -lcurses connect4.c -o connect4

connect4v2: connect4v2.c
	clang -lcurses connect4v2.c -o connect4v2 && ./connect4v2