all:
	gcc game.c -o main -Iinclude -Llib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11