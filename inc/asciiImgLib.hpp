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

namespace cmv
{
    typedef struct {
        int w;
        int h;
    } RESOLUTION;

    std::ostream& operator<<(std::ostream& out, const RESOLUTION& res);

    class Image {
        char* bmp;
        int w;
        int h;
        char* palette;

    public:
        Image(std::string filename, int color);                            // Initializating from file (it can be it ./res folder)
        Image(int width, int height);
        Image(const uint8_t* array, int width, int height, int color);     // Initializating from uint8 rgba array;

        Image();
        Image(const Image& img);
        Image& operator=(const Image& img);

        ~Image();

        RESOLUTION resolution() const;

        const char& operator()(int height, int width) const;

        Image& scale(double scale);
        // /*
        friend ::std::ostream& operator<<(std::ostream& out, Image* img);
        friend ::std::ostream& operator<<(std::ostream& out, Image& img);
        // */
    };

    void reverse(char* array);

}