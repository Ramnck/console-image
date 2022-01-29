#include<asciiImgLib.hpp>
#include<screen.hpp>
#include<conio.h>
#include<vector>
#include<gif_read.h>
#include<ctime>

const char help_message[] = "Usage: %s <file> [<file2> (you can watch several images at the same time with '-ex')]\n[-uwu/-ara/-pix (changes letter palette)]\n[-i (invert colors)]\n[-ex (open new console)]\n[-s <scale size>]\n"; 

void help(char* process_name) {
    printf(help_message, process_name);
    getch();
    exit(1);
}

typedef enum {GIF, IMG, NONE} filetype;

using namespace std;
using namespace cmv;
using namespace fcmd;

int main(int argc, char **argv) {

    int color = 0, speed = 1, external_console = 0;
    filetype mode = NONE;
    double crop = 1;

    vector<string> file_vec;
    file_vec.clear();
    string args;
    
    if (argc == 1) 
        help(argv[0]);

    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "-h" || string(argv[i]) == "--help") help(argv[0]);
        else if (string(argv[i]) == "-i") color += 1;
        else if (string(argv[i]) == "-uwu") color += 2;
        else if (string(argv[i]) == "-ara") color += 4;
        else if (string(argv[i]) == "-pix") color += 6;
        else if (string(argv[i]) == "-ex") {external_console = 1; continue;}
        else if (string(argv[i]) == "-s") {crop = stof(argv[(i++)+1]); continue;}
        else if (strchr(argv[i], '-') == argv[i]) {printf("Wrong flag: %s\n", argv[i]); help(argv[0]); }
        else {file_vec.push_back(argv[i]); continue;}
        args += argv[i];
        args += ' ';
        
    }

    (args += "-s ") += to_string(crop) += ' ';

    if (file_vec.size() > 1 && external_console) {
        vector<HANDLE> hvec;
        for (auto& file : file_vec) {
            STARTUPINFO si;
            PROCESS_INFORMATION pi;

            ZeroMemory( &si, sizeof(si) );
            si.cb = sizeof(si);
            ZeroMemory( &pi, sizeof(pi) );

            char buf[256], proc[64];
            
            memset(proc, ' ', 64);

            strcpy(proc, argv[0]);
            if (!strstr(proc, ".exe"))
                strcat(proc, ".exe");

            string temp_str = proc;
            temp_str += ' '; 
            temp_str += args; 
            temp_str += file;
            strcpy(buf, temp_str.c_str());

            if (!CreateProcess(proc, buf, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
                printf( "CreateProcess failed (%d).\n", GetLastError() );

            hvec.push_back(pi.hProcess);
            hvec.push_back(pi.hThread);
        }
            _getch();
        for (auto& i : hvec)
            TerminateProcess(i, 1);
    } else if (file_vec.size() == 0) { 
        cout << "Error: No files";
        help(argv[1]);
    
    } else if (file_vec.size() == 1 || !external_console) {
        if(file_vec[0].find(".gif") != string::npos) mode = GIF;
        else if(file_vec[0].find(".bmp") != string::npos || file_vec[0].find(".jpeg") != string::npos || file_vec[0].find(".png") != string::npos || file_vec[0].find(".jpg") != string::npos || file_vec[0].find(".tiff") != string::npos ) mode = IMG;
        
        switch (mode)
        {
        case IMG: {
            AsciiImage img(file_vec[0], color);
            Screen s(img.scale(crop), external_console, 15);
            _getch();
            break;
        }
        case GIF: {

            AsciiImage* img_arr;
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

                img_arr = new AsciiImage[num_frames];
                
                for(int i = 0; i < num_frames; i++) 
                    img_arr[i] = (AsciiImage(gif.getFrame(i), width, height, color).scale(crop));
            
            }

            Screen s(img_arr[0], external_console, 30);

            clock_t start;
            double delay = (double)runtime / (double)num_frames / 100.0;

            while (1) for(int i = 0; i < num_frames; i++) {
                start = clock();
                s << img_arr[i];
                if(_kbhit()) 
                    goto escape;
                
                while ((( clock() - start ) / (double) CLOCKS_PER_SEC) < delay);

            }
            escape:

            break;
        }   
        default: {
            printf("\n%s\nError: unsupported file extension", file_vec[0]);
            getch();
            return 1;
            break;
        }
        }
    }

    return 0;
}