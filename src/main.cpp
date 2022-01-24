#include<asciiImgLib.hpp>
#include<screen.hpp>
#include<conio.h>
#include<vector>
#include<gif_read.h>
#include<ctime>

const char help_message[] = "Usage: %s <file> [<file2> (you can watch several images at the same time)]\n[-uwu/-ara/-pix (changes letter palette)]\n[-i (invert colors)]\n[-in (do not open new console (only one file))]\n[-s <scale size>]\n"; 

void help(char* process_name) {
    printf(help_message, process_name);
    exit(1);
}

typedef enum {GIF, BMP, NONE} filetype;

using namespace std;
using namespace cmv;

int main(int argc, char **argv) {

    int color = 0, speed = 1, called_from_console = 1, external_console = 1;
    filetype mode = NONE;
    double crop = 1;

    vector<string> file_vec;
    
    if (argc == 1) 
        help(argv[0]);

    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "-h" || string(argv[i]) == "--help") help(argv[0]);
        else if (string(argv[i]) == "-i") color += 1;
        else if (string(argv[i]) == "-uwu") color += 2;
        else if (string(argv[i]) == "-ara") color += 4;
        else if (string(argv[i]) == "-pix") color += 6;
        else if (string(argv[i]) == "-in") external_console = 0;
        else if (string(argv[i]) == "-s") crop = stof(argv[(i++)+1]);
        else if (strchr(argv[i], '-')) {printf("Wrong flag: %s\n", argv[i]); help(argv[0]); }
        else file_vec.push_back(argv[i]);
    }

    if(file_vec[0].find(".gif") != string::npos) mode = GIF;
    else if(file_vec[0].find(".bmp") != string::npos) mode = BMP;

    switch (mode)
    {
    case BMP: {
        vector<Image> img_vec;
        for (auto& bmp : file_vec)
            img_vec.push_back(Image(bmp, color, crop));

        Screen* last_screen;

        for (auto& img : img_vec) {
            last_screen = new Screen(img, external_console, 15);
        }
        _getch();
        delete last_screen;
        break;
    }
    case GIF: {

        Image* img_arr;
        int width, height, runtime, num_frames;

        {
            FILE* fp = fopen(file_vec[0].c_str(), "rb");
            if(!fp) {
                fp = fopen((string("res/") + file_vec[0]).c_str(), "rb");
                if(!fp) {
                    printf("Error opening gif file\n");
                    return 0;
                }
            }

            fseek(fp, 0, SEEK_END);
            size_t len = ftell(fp);

            uint8_t* gifData = (uint8_t*)malloc(len);
            rewind(fp);
            fread(gifData, len, 1, fp);

            gif_read::GIF gif(gifData);

            free(gifData);
            fclose(fp);
            
            width = gif.getWidth();
            height = gif.getHeight();
            runtime = gif.getTotalTime();
            num_frames = gif.getNumFrames();

            img_arr = new Image[num_frames];
            
            for(int i = 0; i < num_frames; i++) 
                img_arr[i] = (Image(gif.getFrame(i), width, height, color, crop));
        
        }

        Screen s(img_arr[0], external_console, 30);

        clock_t start;
        double delay = (double)runtime / (double)num_frames / 100.0;

        while (1) for(int i = 0; i < num_frames; i++) {
            start = clock();
            s << img_arr[i];
            if(_kbhit()) {
                getch();
                goto end;
            }
            while ((( clock() - start ) / (double) CLOCKS_PER_SEC) < delay);

        }
        end:

        break;
    }   
    default:
        printf("Error: unsupported file extension");
        return 1;
        break;
    }

    return 0;
}