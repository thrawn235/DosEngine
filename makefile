CC=i586-pc-msdosdjgpp-g++
CXXFLAGS=-Wall -O3

TimeEngine.o: TimeEngine.h TimeEngine.cpp
	$(CC) -c TimeEngine.cpp $(CXXFLAGS)

GraphicsEngine.o: GraphicsEngine.h GraphicsEngine.cpp
	$(CC) -c GraphicsEngine.cpp $(CXXFLAGS)

GameEngine.o: GameEngine.h GameEngine.cpp GraphicsEngine.o TimeEngine.o
	$(CC) -c GameEngine.cpp $(CXXFLAGS)

main.exe: TimeEngine.o GraphicsEngine.o GameEngine.o  main.cpp
	$(CC) main.cpp GraphicsEngine.o TimeEngine.o GameEngine.o -o main.exe $(CXXFLAGS)

.PHONY: all
all: main.exe

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.gch
	rm -f *.exe