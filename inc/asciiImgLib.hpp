#pragma once

#include<cstdio>
#include<string>
#include<cmath>
#include<iostream>
#include<windows.h>

#ifndef ASCII_IMAGE_LIBRARY
#define ASCII_IMAGE_LIBRARY
#endif
/*
#ifdef __cplusplus
extern "C" {
#endif
BOOL WINAPI AttachConsole(DWORD dwProcessId);
#ifdef __cplusplus
}
#endif
*/
typedef struct {
    int w;
    int h;
} RESOLUTION;

std::ostream& operator<<(std::ostream& out, const RESOLUTION& res);

class Image {
	char* bmp;
	int w;
	int h;
public:
    char* palette;

    Image(std::string filename, int color, double crop);
    Image(int width, int height);

    Image() = delete;
    Image(Image&) = delete;
    Image& operator=(Image&) = delete;

	~Image();

    RESOLUTION resolution() const;

    const char& operator()(int height, int width) const;

    Image& scale(double scale);
    // Image& scale(double scale);

    friend std::ostream& operator<<(std::ostream& out, Image* img);
    friend std::ostream& operator<<(std::ostream& out, Image& img);
    // friend Image* setConsole(Image* img);
};

void reverse(char* array);