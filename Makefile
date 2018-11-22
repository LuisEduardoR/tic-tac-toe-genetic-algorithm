SRC = ./src/play.cpp ./src/pop.cpp ./src/game.cpp
HEADERS = ./src/pop.h ./src/game.h
OBJECT = ./play.o ./pop.o ./game.o
all:
	g++ $(SRC) $(HEADERS) -c
	g++ $(OBJECT) -o play -lpthread -O3
clean:
	rm -rf *.o
