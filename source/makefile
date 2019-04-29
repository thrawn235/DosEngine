CC=i586-pc-msdosdjgpp-g++
CXXFLAGS=-Wall -O3

InputEngine.o: InputEngine.h InputEngine.cpp
	$(CC) -c InputEngine.cpp $(CXXFLAGS)

TimeEngine.o: TimeEngine.h TimeEngine.cpp
	$(CC) -c TimeEngine.cpp $(CXXFLAGS)

GraphicsEngine.o: GraphicsEngine.h GraphicsEngine.cpp
	$(CC) -c GraphicsEngine.cpp $(CXXFLAGS)

GameEngine.o: GameEngine.h GameEngine.cpp GraphicsEngine.o TimeEngine.o InputEngine.o
	$(CC) -c GameEngine.cpp $(CXXFLAGS)

GameObject.o: GameObject.h GameObject.cpp GameEngine.o
	$(CC) -c GameObject.cpp $(CXXFLAGS)

main.exe: TimeEngine.o GraphicsEngine.o GameEngine.o InputEngine.o GameObject.o  main.cpp
	$(CC) main.cpp GraphicsEngine.o TimeEngine.o GameEngine.o InputEngine.o GameObject.o -o main.exe $(CXXFLAGS)
	mv main.exe ../

.PHONY: all
all: main.exe

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.gch
	rm -f *.exe
	rm -f ../main.exe