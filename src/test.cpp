#include<screen.hpp>

#include<conio.h>
using namespace std;

DWORD Screen::current_pid;

int main(int argc, char** argv) {

    
    
    Screen sc1(40, 20, 1, 15);
    sc1 << "niggers\"churki" << endl << "a";
    int lpid = sc1.current_pid;
    Screen sc2(60, 30, 1, 15);
    sc2 << "hola get up";
    sc1 << "yea it works";
    sc2 << endl << (int)lpid<< endl << (int)sc2.current_pid; 
    _getch();
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