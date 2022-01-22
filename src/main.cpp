#include<asciiImgLib.hpp>
#include<screen.hpp>
#include<conio.h>
#include<vector>

using namespace std;

int main(int argc, char **argv) {

    int color = 0, speed = 1, called_from_console = 1, external_console = 1;
    double crop = 1;
    vector<string> bmp_vec;
    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "-i") color += 1;
        else if (string(argv[i]) == "-uwu") color += 2;
        else if (string(argv[i]) == "-ara") color += 4;
        else if (string(argv[i]) == "-pix") color += 6;
        else if (string(argv[i]) == "-in") external_console = 0;
        else if (string(argv[i]) == "-c") crop = stof(argv[(i++)+1]);
        else if (strchr(argv[i], '-')) {printf("wrong flag: %s\n", argv[i]); return 0;}
        else bmp_vec.push_back(argv[i]);
    }
    /*
    if (bmp == "") {
        cout << "Name of bmp to display: \n";
        called_from_console = 0;
        getline(cin, bmp);
        string str("");
        if (str == "1") speed = 1;
        else if (str == "-uwu") color += 2;
        else if (str == "-ara") color += 4;
    }
    
    Image img(bmp, color);

    Screen s (img.scale(crop), external_console, 15);

    s << img;
    */
    vector<Image*> img_vec;
    for (auto& bmp : bmp_vec)
        img_vec.push_back(new Image(bmp, color, crop));
    // vector<Screen*> scr_vec;
    for (auto& img : img_vec) {
        *(new Screen(*img, external_console, 15)) << *img;
    }


    _getch();
    return 0;
}