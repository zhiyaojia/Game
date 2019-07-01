all: maze

maze: main.cpp game.o board.o pathsolver.o userInterface.o
	g++ -std=c++11 -g -Wall main.cpp game.o board.o pathsolver.o userInterface.o -o maze

board.o: board.h board.cpp
	g++ -std=c++11 -g -Wall -c board.cpp -o board.o

pathsolver.o: pathsolver.h pathsolver.cpp
	g++ -std=c++11 -g -Wall -c pathsolver.cpp -o pathsolver.o

game.o: game.h game.cpp 
	g++ -std=c++11 -g -Wall -c game.cpp -o game.o

userInterface.o: userInterface.h userInterface.cpp
	g++ -std=c++11 -g -Wall -c userInterface.cpp -o userInterface.o