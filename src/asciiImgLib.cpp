#include<asciiImgLib.hpp>

char gradient[] = " .:!/rl1Z4H9W8$@";
char uwugrad[] =  " .:^uwUW";
char aragrad[] =  " .:^raAR";

char c = 0;

void reverse(char* array) {
    int len = strlen(array);
    char temp;
    for(int i = 0; i < len / 2; i++) {
        temp = array[len - i - 1];
        array[len - 1 - i] = array[i];
        array[i] = temp;
    }
}

std::ostream& operator<<(std::ostream& out, const RESOLUTION& res) {
    out << "Width: " << res.w << " Height: " << res.h;
}

Image::Image(int width, int height) : w(width), h(height), bmp(0), palette(0) {}

Image::~Image() { if(bmp) delete[] bmp; if(palette) delete[] palette; }

Image::Image(std::string filename, int color = 0, double crop = 1.0) : w(), h(), bmp(0), palette(0) {
    if (filename.find(".bmp") == std::string::npos) filename += ".bmp";
    FILE * file = fopen(filename.c_str(), "rb");
    if (!file) {
        file = fopen( ((std::string)"res/" + filename).c_str(), "rb");
        if(!file) {
            printf("%s does not exist", filename.c_str()); 
            exit(1);
        }
    }
    char header[2];
    fread(header, 2, 1, file);
    // printf("%d\n", c++);

    if (header[0] == 'B' && header[1] == 'M') {
        // printf("im in \t %d\n", c++);
        short d;
        fseek(file, 18, SEEK_SET);
        int width, height;
        fread(&width, 4, 1, file);
        fread(&height, 4, 1, file);
        fseek(file, 28, SEEK_SET);
        fread(&d, 2, 1, file);

        // Image* output;
        if (d == 1) {
            // if (width != 128 && width != 16 || height != 16 && height != 64) {printf("ERROR: wrong resolution in file %s", filename.c_str()); exit(1);}
            bmp = new char[width * height];
            w = width;
            h = height;
            palette = new char[3];
            palette[0] = -37;
            palette[1] = ' ';
            palette[2] = 0;
            fseek(file, 62, SEEK_SET);
            for (int h = height - 1; h >= 0; h--) {
                char byte, bytes_to_skip = (4 - ((int)std::ceil( (double)width / 8.0 ) % 4)) % 4;
                int bits_left;
                bits_left = width;
                while (bits_left > 0) {
                    fread(&byte, 1, 1, file);
                    int x = 8 - std::min(8, (int)bits_left);
                    for(int bit_mask = std::pow(2,7); bit_mask >= std::pow(2, x); bit_mask /= 2)
                        // *((*output)(h,width - bits_left--)) = byte & bit_mask ? ' ' : -37;
                        bmp[h * width + width - bits_left--] = palette[byte & bit_mask];

                }
                fseek(file, bytes_to_skip, SEEK_CUR);
            }
            
        } else if (d == 4) {
            width *= 2;
            bmp = new char[width * height + 1];
            w = width;
            h = height;
            palette = new char[65];
            switch (color / 2)
            {
            case 0: 
                strcpy(palette, gradient);
                break;
            case 1:
                strcpy(palette, uwugrad);
                break;
            case 2:
                strcpy(palette, aragrad);
                break;
            case 3:
                //  .:!░▒▓█
                /*
                palette[0] = ' ';
                palette[1] = (char)176; // ░
                palette[2] = (char)177; // ▒
                palette[3] = (char)178; // ▓
                palette[4] = (char)219; // █
                palette[5] = 0;
                */
                palette[0] = ' ';
                palette[1] = (char)176; // ░
                palette[2] = (char)178; // ▓
                palette[3] = (char)219; // █
                palette[4] = 0;
                break;
            default:
                printf("Color error\n");
                exit(1);
                break;
            }
            double grad_scale = strlen(gradient) / (double)strlen(palette);
            if (color % 2) reverse(palette);
            fseek(file, 118, SEEK_SET);
            for (int _h = height - 1; _h >= 0; _h--) { 
                char byte, bytes_to_skip = (4 - ((int)std::ceil((double)(width/2) / 2.0) % 4)) % 4;
                // for(int _w = 0; _w < width; _w+=2) {
                for(int _w = 0; _w < width; _w+=4) {
                    fread(&byte, 1, 1, file);
                    bmp[_h * w + _w] = palette[int(((byte & 0b11110000) >> 4) / grad_scale)];
                    
                    // bmp[_h * w + _w + 1] =  palette[int((byte &0b1111) / grad_scale)]; /*

                    bmp[_h * w + _w + 1] = bmp[_h * w + _w];
                    bmp[_h * w + _w + 2] =  palette[int((byte &0b1111) / grad_scale)];
                    bmp[_h * w + _w + 3] = bmp[_h * w + _w + 2];
                    // */
                }
                fseek(file, bytes_to_skip, SEEK_CUR);
            }
        } else {
            printf("Wrong color depth\n");
            fclose(file);
            exit(1);
        }
    } else {
        printf("Wrong file format");
        fclose(file);
        exit(1);
    }
    this->scale(crop);
}


const char& Image::operator()(int height, int width) const { 
    if (height < 0 || width < 0 || height > h || width > w) {
        FreeConsole();
        AttachConsole((DWORD)-1);
        // throw std::runtime_error("Wrong image pixel");
    }
    return *(bmp + w * height + width); 
}


Image& Image::scale(double scale) {
    if (!bmp) return *this;
    if (scale == 1.0) return *this;

    const Image& img = *this;
    int width = (double)w / scale, height = (double)h / scale;
    char* new_buf = new char[width * height];

    for (int _h = 0; _h < height; _h++)
        for (int _w = 0; _w < width; _w++)
            new_buf[_h * width + _w] = img( int(_h * scale), int(_w * scale) );
    delete[] bmp;
    w = width;
    h = height;
    bmp = new_buf;
    return *this;
}
/*
Image& Image::scale(double scale) {
    Image& img = *this;
    int width = (double)w / scale, height = (double)h / scale, iscale;
    char* new_buf = new char[width * height];

    for (int _h = 0; _h < height; _h++) {
        for (int _w = 0; _w < width; _w++) {
            int sum = 0;
            for (int __h = (_h * scale); __h < std::min(h, int(_h * scale) + iscale); __h++)
                for (int __w = (_w * scale); __w < std::min(w, int(_w * scale) + iscale); __w++) 
                    sum += img(std::min(__h, h), std::min(w, __w));
            new_buf[_h * width + _w] = palette[int(sum / scale / scale)];
        }
    }
    delete[] bmp;
    w = width;
    h = height;
    bmp = new_buf;
    return *this;
}
*/
RESOLUTION Image::resolution() const {
    // std::cout << w << h;
    RESOLUTION temp{w,h};
    return temp;
}


std::ostream& operator<<(std::ostream& out, Image& img) {
    img.bmp[img.w*img.h] = '\0'; 
    out << img.bmp;
    return out;
}
std::ostream& operator<<(std::ostream& out, Image* img) {
    for(int i = 0; i < img->h; i++) {
        fwrite(&(*img)(i, 0), img->w, 1, stdout); 
        putchar('\n');
    }
    return out;
}