#pragma once

#include<cstdio>
#include<cstdlib>
#include<string>
#include<cmath>
#include<iostream>
#include<conio.h>
#include<cstring>
#include<windows.h>

#define ASCII_IMAGE_LIBRARY

char gradient[] = " .:!/rl1Z4H9W8$@";
char uwugrad[] =  " .:^uwUW";
char aragrad[] =  " .:^raAR";

class Image {
    char* palette;
	char* bmp;
	int w;
	int h;
public:
    std::pair<int,int> resolution();
    char& operator()(int height, int width);
    Image* crop(double crop);
    Image& scale(double scale);
    Image(int width, int height);
	~Image();
    
    Image(std::string filename, int color = 0);
    friend std::ostream& operator<<(std::ostream& out, Image* img);
    friend std::ostream& operator<<(std::ostream& out, Image& img);
    friend Image* setConsole(Image* img);
};

void reverse(char* array);