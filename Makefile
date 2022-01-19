CC=g++
CFLAGS=-O2 -s -g0 -DNDEBUG -I inc
SRCS=$(shell dir /b /s *.cpp)
all:
	$(CC) $(CFLAGS) $(SRCS) -o bmp
test:
	$(CC) -I inc src/screen.cpp src/test.cpp -o test 