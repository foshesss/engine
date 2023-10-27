build:
	gcc -Wall -std=c99 ./src/*.c -I./include/ -lm -ldl -lSDL2 -o game

run:
	./game

clean:
	rm game
