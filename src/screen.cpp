
#include<asciiImgLib.hpp>

#include<screen.hpp>

using namespace fcmd;

bool Screen::origins_initialisated = false;

double scr_w = GetSystemMetrics(SM_CXSCREEN);
double scr_h = GetSystemMetrics(SM_CYSCREEN);

void setFont(int size) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof( fontInfo );
    GetCurrentConsoleFontEx(hConsole, 0, &fontInfo);
    
    size = std::max(2, size);

    fontInfo.dwFontSize.Y = size;
    fontInfo.dwFontSize.X = size / 2;
    
    SetCurrentConsoleFontEx(hConsole, TRUE, &fontInfo); 
}

void setConsole(int w, int h) {
    std::string text = "mode con cols=";
    text += std::to_string(w);
    text += " lines=";
    text += std::to_string(h);
    system(text.c_str());
}

void Screen::orig_size_init() {
    
    origins_initialisated = true;
    
    _COORD coord = {height, width};
    _SMALL_RECT Rect = {0,0,coord.X - 1, coord.Y - 1};
    CONSOLE_SCREEN_BUFFER_INFO csbiData;
    GetConsoleScreenBufferInfo(GetStdHandle (STD_OUTPUT_HANDLE), &csbiData);
    orig_size.X = csbiData.dwSize.X;
    orig_size.Y = csbiData.srWindow.Bottom - csbiData.srWindow.Top + 1;

    ZeroMemory(&orig_font, sizeof(orig_font));
    orig_font.cbSize = sizeof( orig_font );
    GetCurrentConsoleFontEx(GetStdHandle (STD_OUTPUT_HANDLE), 0, &orig_font);
}

void Screen::last_part_of_init(int font) {

    font = std::min( font, (int) std::min( scr_w / (double)width, scr_h / 2 / (double)height) );

    if (external_console) {
        FreeConsole();
        AllocConsole();
    }

    _COORD coord = {height, width};
    _SMALL_RECT Rect = {0,0,coord.X - 1, coord.Y - 1};

    setFont(font);
    setConsole(width, height);
    
    buffer = new char[height*width];
    buf_handler = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); 

    SetConsoleActiveScreenBuffer(buf_handler);
    SetConsoleWindowInfo(buf_handler, TRUE, &Rect);
    SetConsoleScreenBufferSize(buf_handler, coord);

}

Screen::Screen(int _width, int _height, int _external_console, int font = 15) :
            width(_width), height(_height), orig_font(),
            orig_size(), external_console(_external_console),
            buffer(), buf_handler(), bytes_written(), pointer({0,0}) {
    if(!origins_initialisated)
        orig_size_init();
    last_part_of_init(font);

    }

Screen::~Screen() {
    if (buffer) delete[] buffer;
    if (external_console) {
        FreeConsole();
        AttachConsole((DWORD)-1);
    }
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleActiveScreenBuffer(Handle);
    SetCurrentConsoleFontEx(Handle, TRUE, &orig_font);
    if (!external_console)
        setConsole(orig_size.X, orig_size.Y);
    else {
        _SMALL_RECT Rect = {0, 0, orig_size.X - 1, orig_size.Y - 1};
        SetConsoleWindowInfo(Handle, TRUE, &Rect);
        SetConsoleScreenBufferSize(Handle, orig_size);
    }
}

#ifdef ASCII_IMAGE_LIBRARY

Screen::Screen(cmv::AsciiImage& img, int _external_console, int font = 15) : 
            width(0), height(0), orig_font(), 
            orig_size(), external_console(_external_console),
            buffer(0), buf_handler(), bytes_written(), pointer({0,0}) {


    cmv::RESOLUTION r = img.resolution();

    if (r.w > scr_w - 10 || r.h > scr_h - 10) 
        img.scale(std::max((double)r.w / (scr_w - 20.0), (double)r.h / (scr_h / 2.0 - 20.0)));

    r = img.resolution();
    width = r.w;
    height = r.h;

    if (!origins_initialisated)
        orig_size_init();

    last_part_of_init(font);

    (*this) << img;
}

Screen& Screen::operator<<(cmv::AsciiImage& input) {

    cmv::RESOLUTION r = input.resolution();

    if (r.w > scr_w - 10 || r.h > scr_h - 10) 
        input.scale(std::max((double)r.w / (scr_w - 20.0), (double)r.h / (scr_h / 2.0 - 20.0)));

    r = input.resolution();

    
    int img_w = r.w, img_h = r.h,CRLF = 0;


    if(img_w == width && img_h == height) { 
        memcpy(buffer, &input(0,0), width * height);
        this->display();
        return *this;
    }

    if (img_w > width) img_w = width;
    else if (img_w < width) CRLF = 1;
    if(img_h > height - pointer.Y - 1) (*this) << clrs;
    if (CRLF) {
        for (int i = 0; i < img_h; i++)
            memcpy(&this->pix(), &input(i, 0), img_w);
    } else {
        memcpy(&this->pix(), &input(0, 0), img_w * img_h);
    }
    this->display();
    return *this;
}

#endif /* ASCII_IMAGE_LIBRARY */

Screen& Screen::operator<<(char input) {

    switch (input)
    {
    case '\n': case '\0':
        (*this) << endl;
        break;
    case '\t':
        (*this) << "\\t is forbidden";
        break;
    case '\b':
        mp(-1);
        break;
    case '\r':
        pointer.X = 0;
        break;
    default:
        this->pix() = input;
        mp(0, 1);
        break;
    }
    
    this->display();
    return *this;
}

Screen& Screen::operator<<(command cmd) {

    switch (cmd)
    {
    case endl:
        pointer.X = 0;
        if(pointer.Y++ == height - 1)
            roll(1);
            break;
    case clrs:
        clear();
        break;
    case retc:
        pointer.X = 0;
        break;
    default:
        break;
    }
    this->display();
    return *this;
}

Screen& Screen::operator<<(std::string input) {

    int CRLF = 0;
    if(input.find('\b') != input.npos || input.find('\t') != input.npos || input.find('\n') != input.npos || input.find('\r') != input.npos) {
        memcpy(&this->pix(), input.c_str(), input.length());
        mp(input.length());
        this->display();
    } else 
        for (auto ch : input)
            (*this) << ch;
    return *this;
}

Screen& Screen::operator<<(cmv::RESOLUTION r) {
    Screen& s = *this;
    s << "W: " << r.w << " H: " << r.h;
    return s;
}

Screen& Screen::operator<<(const char* input) {
    
    return (*this) << std::string(input);
}

Screen& Screen::operator<<(int input) {
    return (*this) << std::to_string(input);
}

Screen& Screen::operator<<(double input) {
    (*this) << std::to_string(input);
}

void Screen::roll(int _height) {
    memcpy(buffer, &pix(_height, 0), (height - std::min(_height, height)) * width);
    memset(buffer + (height - _height) * width, ' ', (height - _height) * width);
    pointer.Y -= std::min(_height, height);
}

COORD& Screen::mp(int length) {
    pointer.X += length;
    if (pointer.Y > height - 1) {
        roll(std::min(length, height));
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

char& Screen::pix(int _height, int _width = 0) {
    return *(buffer + _height * width + _width);
}

char& Screen::pix() {
    return this->pix(pointer.Y, pointer.X);
}

void Screen::clear() {
    pointer.X = 0;
    pointer.Y = 0;
    memset(buffer, ' ', width*height);
}