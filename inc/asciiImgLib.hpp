#pragma once

#ifndef ASCII_IMAGE_LIBRARY
#define ASCII_IMAGE_LIBRARY

#include<cstdio>
#include<string>
#include<cmath>
#include<iostream>
#include<windows.h>

namespace cmv
{
    typedef struct {
        int w;
        int h;
    } RESOLUTION;

#pragma pack(push, 1)
    typedef struct {
        char r;
        char g;
        char b;
    } RGB;

    typedef struct {
        char r;
        char g;
        char b;
        char a;
    } RGBA;
#pragma pack(pop)

    #ifdef _GLIBCXX_IOSFWD
    std::ostream& operator<<(std::ostream& out, const RESOLUTION& res);
    #endif

    class AsciiImage {

    protected:
        char* bmp;
        int w;
        int h;
        char* palette;

    public:
        AsciiImage(std::string filename, int color);                            // Initializating from file (it can be it ./res folder)
        AsciiImage(int width, int height);
        AsciiImage(const uint8_t* array, int width, int height, int color);     // Initializating from uint8 rgba array;

        AsciiImage();
        AsciiImage(const AsciiImage& img);
        AsciiImage& operator=(const AsciiImage& img);

        ~AsciiImage();

        RESOLUTION resolution() const;

        const char& operator()(int height, int width) const;

        AsciiImage& scale(double scale);
        // /*
        #ifdef _GLIBCXX_IOSFWD
        friend ::std::ostream& operator<<(std::ostream& out, AsciiImage* img);
        friend ::std::ostream& operator<<(std::ostream& out, AsciiImage& img);
        #endif
        // */
    };

    void reverse(char* array);

}
#endif  /*  ASCII_IMAGE_LIBRARY  */