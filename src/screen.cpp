// #include<asciiImgLib.hpp>

#include<screen.hpp>

typedef struct _CONSOLE_FONT_INFOEX
{
    ULONG cbSize;
    DWORD nFont;
    COORD dwFontSize;
    UINT  FontFamily;
    UINT  FontWeight;
    WCHAR FaceName[LF_FACESIZE];
}CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;
//the function declaration begins
#ifdef __cplusplus
extern "C" {
#endif
BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
BOOL WINAPI GetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
BOOL WINAPI AttachConsole(DWORD dwProcessId);
#ifdef __cplusplus
}
#endif

void setFont(int size) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof( fontInfo );
    GetCurrentConsoleFontEx(hConsole, TRUE, &fontInfo);
    
    fontInfo.dwFontSize.Y = size;
    
    SetCurrentConsoleFontEx(hConsole, TRUE, &fontInfo); 
}

Screen::Screen(int _width, int _height, int external_console = 1, int font = 15) : 
            width(_width), height(_height), pid(GetCurrentProcessId()),
            buffer(), buf_handler(), bytes_written(), pointer({0,0}) {
        _COORD coord = {height, width};
        _SMALL_RECT Rect = {0,0,coord.Y - 1, coord.X - 1};
        
        if (external_console) {
            FreeConsole();
            AllocConsole();
        }

        setFont(font);

        HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleScreenBufferSize(Handle, coord);
        SetConsoleWindowInfo(Handle, TRUE, &Rect);

        buffer = new char[height*width];
        buf_handler = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); 

        SetConsoleActiveScreenBuffer(buf_handler);
        this->clear();
        Screen::current_pid = pid;
}

#ifdef ASCII_IMAGE_LIBRARY

Screen::Screen(Image& img, int external_console = 1, int font = 15) {
    std::pair<int,int> temp = img.resolution();
    Screen(temp.first, temp.second);
}

Screen& Screen::operator<<(Image& input) {
    if (Screen::current_pid != pid) { FreeConsole(); AttachConsole(pid); Screen::current_pid = pid;}
    // int img_w = input.resolution().first > width ? width: input.resolution().first, img_h = ? : ;
    int img_w = input.resolution().first, img_h = std::min(input.resolution().second, height), CRLF = 0;
    if (img_w > width) img_w = width;
    else if (img_w < width) CRLF = 1;
    if(img_h > height - pointer.Y - 1) (*this) << clrs;
    if (CRLF) {
        for (int i = 0; i < img_h; i++)
            memcpy(&(*this)(), &input(i, 0), img_w);
    } else {
        memcpy(&(*this)(), &input(0, 0), img_w * img_h);
    }
    return *this;
}


#endif

void Screen::roll(int _height = 1) {
    memcpy(buffer, &(*this)(_height), (height - std::min(_height, height)) * width);
    pointer.Y -= std::min(_height, height);
}

COORD& Screen::mp(int length) {
    pointer.X += length;
    pointer.Y += pointer.X / width;
    // pointer.X = pointer.X % width;
    // pointer.Y = pointer.Y % height;
    if (pointer.Y > height - 1) {
        roll((length / width) % width);
        pointer.Y = height - 1;
    }
    return this->pointer;
}

COORD& Screen::mp(int _height, int _width) {
    this->mp(_height * width + _width);
    return this->pointer;
}

void Screen::display() {
        Screen::buffer[Screen::width * Screen::height - 1] = '\0';
        WriteConsoleOutputCharacter(buf_handler, (LPCSTR) Screen::buffer, Screen::width * Screen::height + 1, { 0, 0 }, &bytes_written);
}

char& Screen::operator()(int _height, int _width = 0) {
    return *(buffer + _height * width + _width);
}

char& Screen::operator()() {
    return (*this)(pointer.Y, pointer.X);
}

void Screen::clear() {
    pointer.X = 0;
    pointer.Y = 0;
    memset(buffer, ' ', width*height);
}

Screen& Screen::operator<<(command cmd) {
    if (Screen::current_pid != pid) { FreeConsole(); AttachConsole(pid); Screen::current_pid = pid;}

    switch (cmd)
    {
    case endl:
        pointer.X = 0;
        mp(0, 0);
        break;
    case clrs:
        clear();
        break;
    case retc:
        pointer.X = 0;
        break;
    // case tab:
        // mp(4);
        // break;
    default:
        break;
    }
    this->display();
    return *this;
}

Screen& Screen::operator<<(const char* input) {
    // if (Screen::current_pid != pid) { FreeConsole(); AttachConsole(pid); Screen::current_pid = pid;}

    (*this) << std::string(input);
    return (*this);
}

Screen& Screen::operator<<(int input) {
    (*this) << std::to_string(input);
    return *this;
}

Screen& Screen::operator<<(char input) {
    if (Screen::current_pid != pid) { FreeConsole(); AttachConsole(pid); Screen::current_pid = pid;}

    switch (input)
    {
    case '\n': case '\0':
        mp(1, -pointer.X);
        break;
    case '\t':
        (*this) << "\\t is forbidden";
        break;
    case '\b':
        mp(-2);
        break;
    case '\r':
        mp(0, -pointer.X);
        break;
    default:
         (*this)() = input;
        mp(1);
        break;
    }
    
    this->display();
    return *this;
}

Screen& Screen::operator<<(std::string input) {
    if (Screen::current_pid != pid) { FreeConsole(); AttachConsole(pid); Screen::current_pid = pid;}

    int CRLF = 0;
    if(input.find('\b') == input.npos || input.find('\t') == input.npos || input.find('\n') == input.npos || input.find('\r') == input.npos) {
        memcpy(&(*this)(), input.c_str(), input.length());
        mp(input.length());
        this->display();
    } else 
        for (auto ch : input)
                (*this) << ch;
    return *this;
}

Screen::~Screen() {
    if (buffer) delete[] buffer;
}
