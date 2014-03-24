all: 2048

2048: main.o Game.o
	g++ main.o Game.o -o 2048 -lncurses

main.o: main.cpp
	g++ -c main.cpp

Game.o: Game.cpp
	g++ -c Game.cpp

clean:
	rm -rf *.o 2048
