#pragma once

#include<string>
#include<cstdio>
#include<Windows.h>

#ifndef MULTI_CONSOLE_LIBRARY
#define MULTI_CONSOLE_LIBRARY
#endif

void setFont(int size);

enum command {endl, clrs, retc};

class Screen {
    COORD pointer;
    int width;
    int height;
    char* buffer;
    HANDLE buf_handler;
    DWORD bytes_written;
    DWORD pid;

    void roll(int height);

    COORD& mp(int length);
    COORD& mp(int _height, int _width);

    public:

    static DWORD current_pid;

    Screen() = delete;
    Screen(Screen&) = delete;

    Screen& operator=(Screen&) = delete;

    #ifdef ASCII_IMAGE_LIBRARY
    Screen(Image& img, int external_console, int font);
    Screen& operator<<(Image& input);
    #endif
    
    Screen(int _width, int _height, int external_console, int font);

    char& operator()();
    char& operator()(int _height, int _width);
    
    Screen& operator<<(const char* input);
    Screen& operator<<(int input);
    Screen& operator<<(std::string input);
    Screen& operator<<(char input);
    Screen& operator<<(command);

    void display();
    void clear();
    ~Screen();
};


