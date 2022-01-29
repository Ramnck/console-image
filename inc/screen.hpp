#pragma once

#include<string>
#include<cstdio>
#include<Windows.h>

#ifndef MULTI_CONSOLE_LIBRARY
#define MULTI_CONSOLE_LIBRARY


void setFont(int size);
void setConsole(int w, int h);

enum command {endl, clrs, retc};

namespace cmv {
    class AsciiImage;
}


namespace fcmd // fast console    
{
    class Screen {
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

        public:

        Screen(int _width, int _height, int external_console, int font);

        #ifdef ASCII_IMAGE_LIBRARY
        Screen(cmv::AsciiImage& img, int external_console, int font);
        Screen& operator<<(cmv::AsciiImage& input);
        Screen& operator<<(cmv::RESOLUTION r);
        #endif

        Screen() = delete;
        Screen(Screen&) = delete;
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

#endif