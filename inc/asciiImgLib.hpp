#pragma once

#include<cstdio>
#include<cstdlib>
#include<string>
#include<cmath>
#include<iostream>
#include<conio.h>
#include<cstring>
#include<windows.h>

#ifndef ASCII_IMAGE_LIBRARY
#define ASCII_IMAGE_LIBRARY

#endif

typedef struct {
    int w;
    int h;
} Resolution;


class Image {
    char* palette;
	char* bmp;
	int w;
	int h;
public:
    Resolution resolution();
    char& operator()(int height, int width);
    Image* crop(double crop);
    Image& scale(double scale);
    Image(int width, int height);
	~Image();
    Image() = delete;
    Image(Image&) = delete;
    Image& operator=(Image&) = delete;
    Image(std::string filename, int color = 0);
    friend std::ostream& operator<<(std::ostream& out, Image* img);
    friend std::ostream& operator<<(std::ostream& out, Image& img);
    friend Image* setConsole(Image* img);
};

void reverse(char* array);