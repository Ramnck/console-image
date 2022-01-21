
#include<asciiImgLib.hpp>

#include<screen.hpp>

// #include<iostream>
#include<conio.h>
using namespace std;

// DWORD Screen::current_pid;

int main(int argc, char** argv) {
    

    int scr_w = GetSystemMetrics(SM_CXSCREEN);
    int scr_h = GetSystemMetrics(SM_CYSCREEN); 

    // cout << scr_h << ' ' << scr_w;

    Image img(argc > 1 ? argv[1] : "gojo", 0);

    // cout << img;

    Screen s(img.crop(1), 0, 15);

    s << img;
    // s << img;
    // FreeConsole();
    // AttachConsole((DWORD)-1);


    // CONSOLE_SCREEN_BUFFER_INFO csbiData;

    // GetConsoleScreenBufferInfo(GetStdHandle (STD_OUTPUT_HANDLE), &csbiData);

    // s.orig_size.X = csbiData.dwSize.X;
    // s.orig_size.Y = csbiData.dwSize.Y + 1;

    // printf("%d\t%d\t%d\t%d\n", s.orig_size.X, csbiData.dwSize.Y, csbiData.srWindow.Bottom, csbiData.srWindow.Top);
    // printf("X:%d Y: %d\n", s.orig_font.dwFontSize.X, s.orig_font.dwFontSize.Y);
    // double asd = (double(s.orig_rect.Right) - double(s.orig_rect.Left)) / (double)s.orig_font.dwFontSize.X;
    // double asd1 = (double(s.orig_rect.Right) - double(s.orig_rect.Left)) / ((double)s.orig_font.dwFontSize.X + 1);
    // printf("%f if x+1: %f", asd, asd1);
    // cout << "img done";
    
    // s << "asdas" << " " << "asd" << ::endl << "second " << 10;
    // s << 'a';
    // s << img;
    // for (int i = 1000 - 7, a = 0; i > 7; i-=7, a++) {
        // s << a << " : "<< i+7  << " - 7 = " << i << ::endl;
        // Sleep(10);}
    // sc1 << img;
    // int lpid = sc1.current_pid;
    // Screen sc2(60, 30, 1, 15);
    // sc2 << "hola get up";
    
    // cout << img;
    // cout << s.testInfo().first << " " << s.testInfo().second;
    // printf("not in proper way\n");
    // sc1 << "yea it works";
    // printf("truely");
    // sc2 << endl << (int)lpid<< endl << (int)sc2.current_pid;
    // while (1)
    getch();
    // for(int i = 0; i < 100; i += 10) {
        // Screen scr(i,i,1,15);
        // scr << "niggers";
        // _getch();
    // }
    // while(_getch())
        // scr << endl << 'a';
    // scr.display();
    // _getch();

    return 0;
}

