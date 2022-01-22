CC=g++
CFLAGS=-O2 -s -g0 -DNDEBUG -w -I inc
SRCS=$(shell dir /b /s *.cpp)
all:
	$(CC) $(CFLAGS) $(SRCS) -o bmp
test:
	$(CC) -w -I inc src/screen.cpp src/test.cxx src/asciiImgLib.cpp -o test 