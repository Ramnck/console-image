#include<asciiImgLib.hpp>
#include<screen.hpp>


void setConsole(int w, int h) {
    std::string text = "mode con cols=";
    text += std::to_string(w);
    text += " lines=";
    text += std::to_string(h+2);
    
    system(text.c_str());
    // COORD crd = {img->w, img->h + 2};
    // SMALL_RECT src = {0, 0, crd.X, crd.Y};
    // SetConsoleScreenBufferSize (GetStdHandle (STD_OUTPUT_HANDLE), crd);
    // SetConsoleWindowInfo (GetStdHandle (STD_OUTPUT_HANDLE), TRUE, &src);

    // console_handler = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); 
	// SetConsoleActiveScreenBuffer(console_handler);

}

using namespace std;

int main(int argc, char **argv) {

    int color = 0, speed = 1, called_from_console = 1, print_with_CR = 0, external_console = 1;
    double crop = 1;
    string bmp("");
    // printf("%d\n", c++);
    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "-i") color += 1;
        else if (string(argv[i]) == "-uwu") color += 2;
        else if (string(argv[i]) == "-ara") color += 4;
        else if (string(argv[i]) == "-pix") color += 6;
        else if (string(argv[i]) == "-s") speed = 0;
        else if (string(argv[i]) == "-in") external_console = 0;
        else if (string(argv[i]) == "-c") crop = stof(argv[(i++)+1]);
        else if (string(argv[i]) == "-CR") print_with_CR = 1;
        else if (strchr(argv[i], '-')) {printf("wrong flag: %s\n", argv[i]); return 0;}
        else bmp = argv[i];
    }
    // printf("%d\n", c++);
    if (bmp == "") {
        cout << "Name of bmp to display: \n";
        called_from_console = 0;
        getline(cin, bmp);
        // cout << "Type 1 to display quickly 0 to display slow :\n";
        string str("");
        // getline(cin, str);
        if (str == "1") speed = 1;
        else if (str == "-uwu") color += 2;
        else if (str == "-ara") color += 4;
    }
    
    // setFont(1);

    

    // if (print_with_CR)
    //     delete fileToArray(bmp, color, crop)->printVK(speed);
    // else
    //     delete fileToArray(bmp, color, crop)->print(speed);

    _getch();
    return 0;
}