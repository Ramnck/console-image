#pragma once

#ifndef MULTI_CONSOLE_LIBRARY
#define MULTI_CONSOLE_LIBRARY

#include <string>
#include <Windows.h>

#include<conio.h>

void setFont(int size);
void setConsole(int w, int h);

enum command {endl, clrs, retc};

namespace cmv {
    class AsciiImage;
}


namespace fcmd // fast console    
{
    class Screen {
    protected:
        COORD pointer;
        int width;
        int height;
        char* buffer;
        HANDLE buf_handler;
        DWORD bytes_written;
        bool external_console;
        COORD orig_size;
        CONSOLE_FONT_INFOEX orig_font;
        static bool origins_initialisated;

        void orig_size_init();
        void last_part_of_init(int font);

        void roll(int height);

        COORD& mp(int length);
        COORD& mp(int _height, int _width);
        
        char& pix();
        char& pix(int _height, int _width);

        // Screen& fit(int window_width, int window_height);

    public:

        Screen();
        Screen(int _width, int _height, int external_console, int font);

        #ifdef ASCII_IMAGE_LIBRARY
        Screen(cmv::AsciiImage& img, int external_console, int font);
        Screen(cmv::AsciiImage&& img, double scale, int external_console, int font);
        Screen& operator<<(cmv::AsciiImage& input);
        Screen& operator<<(cmv::AsciiImage&& input);
        #endif

        Screen& operator=(Screen&& scr);
        Screen& operator=(Screen&) = delete;

        ~Screen();
        
        Screen& operator<<(std::string input);
        Screen& operator<<(char input);
        Screen& operator<<(command);

        Screen& operator<<(double input);
        Screen& operator<<(const char* input);
        Screen& operator<<(int input);
        

        void display();
        void clear();
    
    };
    
}

#endif /*  MULTI_CONSOLE_LIBRARY  */