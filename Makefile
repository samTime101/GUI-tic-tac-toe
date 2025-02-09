# gameoflife:
#	gcc main.c -o main ../../lib/libraylib.a -I ../../include -lGL -lm -ldl -lpthread
make:
	gcc tic-tac-toe.c -o game ../../lib/libraylib.a -I ../../include -lGL -lm -ldl -lpthread && ./game
