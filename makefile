CC=i586-pc-msdosdjgpp-g++
CXXFLAGS=-Wall -O3
SUBDIR = ./source/

$(SUBDIR)InputEngine.o: $(SUBDIR)InputEngine.h $(SUBDIR)InputEngine.cpp
	$(CC) -c $(SUBDIR)InputEngine.cpp -o $(SUBDIR)InputEngine.o $(CXXFLAGS)

$(SUBDIR)TimeEngine.o: $(SUBDIR)TimeEngine.h $(SUBDIR)TimeEngine.cpp
	$(CC) -c $(SUBDIR)TimeEngine.cpp -o $(SUBDIR)TimeEngine.o $(CXXFLAGS)

$(SUBDIR)GraphicsEngine.o: $(SUBDIR)GraphicsEngine.h $(SUBDIR)GraphicsEngine.cpp
	$(CC) -c $(SUBDIR)GraphicsEngine.cpp -o $(SUBDIR)GraphicsEngine.o $(CXXFLAGS)

$(SUBDIR)GameEngine.o: $(SUBDIR)GameEngine.h $(SUBDIR)GameEngine.cpp $(SUBDIR)GraphicsEngine.o $(SUBDIR)TimeEngine.o $(SUBDIR)InputEngine.o $(SUBDIR)GameObject.o
	$(CC) -c $(SUBDIR)GameEngine.cpp -o $(SUBDIR)GameEngine.o $(CXXFLAGS)

$(SUBDIR)GameObject.o: $(SUBDIR)GameObject.h $(SUBDIR)GameObject.cpp $(SUBDIR)GameEngine.o
	$(CC) -c $(SUBDIR)GameObject.cpp  -o $(SUBDIR)GameObject.o $(CXXFLAGS)

main.exe: $(SUBDIR)TimeEngine.o $(SUBDIR)GraphicsEngine.o $(SUBDIR)GameEngine.o $(SUBDIR)InputEngine.o $(SUBDIR)GameObject.o  $(SUBDIR)main.cpp
	$(CC) $(SUBDIR)main.cpp $(SUBDIR)GraphicsEngine.o $(SUBDIR)TimeEngine.o $(SUBDIR)GameEngine.o $(SUBDIR)InputEngine.o $(SUBDIR)GameObject.o -o main.exe $(CXXFLAGS)

.PHONY: all
all: main.exe

.PHONY: clean
clean:
	rm -f $(SUBDIR)*.o
	rm -f $(SUBDIR)*.gch
	rm -f $(SUBDIR)*.exe
	rm -f main.exe