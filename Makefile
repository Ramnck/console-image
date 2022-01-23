CC=g++
CFLAGS=-s -g0 -DNDEBUG -w 
INC= -I inc
SRCS=$(shell dir /b /s *.cpp)
all:
	$(CC) $(CFLAGS) $(INC) $(SRCS) -o cmv
test:
	$(CC) -w $(INC) src/gif_read.cpp src/screen.cpp src/test.cxx src/asciiImgLib.cpp -o test 
fast:
	$(CC) -w $(INC) src/gif_read.cpp src/screen.cpp src/main.cpp src/asciiImgLib.cpp -o fast