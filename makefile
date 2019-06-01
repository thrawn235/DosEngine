#############################################################################################
#
#	makefile
#
#	by Sebastian Gurlin
#
#
#	Description:
#
#	Makefile for 2DEngine
#	make all, compiles the whole Project
#	all source files are in the ./source subfolder, therefore the awkward $(SUBDIR)
#
#############################################################################################

#========= Variables ============
CC=i586-pc-msdosdjgpp-g++
CXXFLAGS=-Wall -O3 -s
SUBDIR = ./source/
#================================


#============================ compile all objects ===================================
$(SUBDIR)SoundEngine.o: $(SUBDIR)SoundEngine.h $(SUBDIR)SoundEngine.cpp
	$(CC) -c $(SUBDIR)SoundEngine.cpp -o $(SUBDIR)SoundEngine.o $(CXXFLAGS)

$(SUBDIR)InputEngine.o: $(SUBDIR)InputEngine.h $(SUBDIR)Vector2D.h $(SUBDIR)InputEngine.cpp
	$(CC) -c $(SUBDIR)InputEngine.cpp -o $(SUBDIR)InputEngine.o $(CXXFLAGS)

$(SUBDIR)TimeEngine.o: $(SUBDIR)TimeEngine.h $(SUBDIR)Vector2D.h $(SUBDIR)TimeEngine.cpp
	$(CC) -c $(SUBDIR)TimeEngine.cpp -o $(SUBDIR)TimeEngine.o $(CXXFLAGS)

$(SUBDIR)GraphicsEngine.o: $(SUBDIR)GraphicsEngine.h $(SUBDIR)Vector2D.h $(SUBDIR)GraphicsEngine.cpp
	$(CC) -c $(SUBDIR)GraphicsEngine.cpp -o $(SUBDIR)GraphicsEngine.o $(CXXFLAGS)

$(SUBDIR)GameEngine.o: $(SUBDIR)GameEngine.h $(SUBDIR)Vector2D.h $(SUBDIR)GameEngine.cpp $(SUBDIR)TimeEngine.o $(SUBDIR)InputEngine.o $(SUBDIR)SoundEngine.o $(SUBDIR)GameObject.o
	$(CC) -c $(SUBDIR)GameEngine.cpp -o $(SUBDIR)GameEngine.o $(CXXFLAGS)

$(SUBDIR)GameObject.o: $(SUBDIR)GameObject.h $(SUBDIR)Vector2D.h $(SUBDIR)GameObject.cpp $(SUBDIR)HelperFunctions.h #$(SUBDIR)GameEngine.o
	$(CC) -c $(SUBDIR)GameObject.cpp  -o $(SUBDIR)GameObject.o $(CXXFLAGS)
#=====================================================================================


#============================================= compile main exe and link all objects to it =======================================================
main.exe: $(SUBDIR)Vector2D.h $(SUBDIR)TimeEngine.o $(SUBDIR)GraphicsEngine.o $(SUBDIR)GameEngine.o $(SUBDIR)InputEngine.o $(SUBDIR)SoundEngine.o $(SUBDIR)GameObject.o  $(SUBDIR)main.cpp
	$(CC) $(SUBDIR)main.cpp $(SUBDIR)GraphicsEngine.o $(SUBDIR)TimeEngine.o $(SUBDIR)GameEngine.o $(SUBDIR)InputEngine.o $(SUBDIR)SoundEngine.o $(SUBDIR)GameObject.o -o main.exe $(CXXFLAGS)
	strip main.exe
#=================================================================================================================================================

#======= start here =========
.PHONY: all
all: main.exe
#============================

#=========== cleanup ============
.PHONY: clean
clean:
	rm -f $(SUBDIR)*.o
	rm -f $(SUBDIR)*.gch
	rm -f $(SUBDIR)*.exe
	rm -f main.exe
#================================