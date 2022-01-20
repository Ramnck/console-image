#pragma once

#include<string>
#include<cstdio>
#include<Windows.h>

#ifndef MULTI_CONSOLE_LIBRARY
#define MULTI_CONSOLE_LIBRARY
#endif

void setFont(int size);
void setConsole(int w, int h);

enum command {endl, clrs, retc};

class Image;

// const DWORD main_pid = GetCurrentProcessId(); 

typedef struct _CONSOLE_FONT_INFOEX
{
    ULONG cbSize;
    DWORD nFont;
    COORD dwFontSize;
    UINT  FontFamily;
    UINT  FontWeight;
    WCHAR FaceName[LF_FACESIZE];
}CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;

#ifdef __cplusplus
extern "C" {
#endif
BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
BOOL WINAPI GetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
BOOL WINAPI AttachConsole(DWORD dwProcessId);
#ifdef __cplusplus
}
#endif

class Screen {
    COORD pointer;
    int width;
    int height;
    char* buffer;
    HANDLE buf_handler;
    DWORD bytes_written;
    // PROCESS_INFORMATION pinf;

    void roll(int height);

    COORD& mp(int length);
    COORD& mp(int _height, int _width);
    
    char& pix();
    char& pix(int _height, int _width);

    public:
    
    COORD orig_size;
    CONSOLE_FONT_INFOEX orig_font;

    // static DWORD current_pid;

    Screen() = delete;
    Screen(Screen&) = delete;

    Screen& operator=(Screen&) = delete;
    
    Screen(int _width, int _height, int external_console, int font);


    // #ifdef ASCII_IMAGE_LIBRARY
    Screen(Image& img, int external_console, int font);
    Screen& operator<<(Image& input);
    // #endif
    
    // char& operator()();
    // char& operator()(int _height, int _width);
    
    Screen& operator<<(const char* input);
    Screen& operator<<(int input);
    Screen& operator<<(std::string input);
    Screen& operator<<(char input);
    Screen& operator<<(command);

    void display();
    void clear();
    ~Screen();
};


