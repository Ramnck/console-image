#include<asciiImgLib.hpp>

void reverse(char* array) {
    int len = strlen(array);
    char temp;
    for(int i = 0; i < len / 2; i++) {
        temp = array[len - i - 1];
        array[len - 1 - i] = array[i];
        array[i] = temp;
    }
}

Image::Image(std::string filename, int color = 0) {
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
    if (header[0] == 'B' && header[1] == 'M') {
        short d;
        fseek(file, 18, SEEK_SET);
        int width, height;
        fread(&width, 4, 1, file);
        fread(&height, 4, 1, file);
        fseek(file, 28, SEEK_SET);
        fread(&d, 2, 1, file);

        // Image* output;
        if (d == 1) {
            Image(width, height);
            // if (width != 128 && width != 16 || height != 16 && height != 64) {printf("ERROR: wrong resolution in file %s", filename.c_str()); exit(1);}
            palette = new char[3];
            bmp = new char[width*height];
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
            Image(width, height);
            palette = new char[40];
            switch (color)
            {
            case 0: case 1:
                strcpy(palette, gradient);
                break;
            case 2: case 3:
                strcpy(palette, uwugrad);
                break;
            case 4: case 5:
                strcpy(palette, aragrad);
                break;
            case 6: case 7:
                //  .:!░▒▓█
                palette[0] = ' ';
                palette[1] = (char)176; // ░
                palette[2] = (char)177; // ▒
                palette[3] = (char)178; // ▓
                palette[4] = (char)219; // █
                palette[5] = 0;
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
                for(int _w = 0; _w < width; _w+=4) {
                    fread(&byte, 1, 1, file);
                    bmp[_h * w + _w] = palette[int(((byte & 0b11110000) >> 4) / grad_scale)];
                    bmp[_h * w + _w + 1] = bmp[_h * w + _w];
                    bmp[_h * w + _w + 2] =  palette[int((byte &0b1111) / grad_scale)];
                    bmp[_h * w + _w + 3] = bmp[_h * w + _w + 2];
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
    
}



char& Image::operator()(int height, int width = 0) { return *(bmp + w * height + width); }

Image::Image(int width, int height) : w(width), h(height), bmp() {
    bmp = new char[width*height + 1]; 
    memset(bmp, ' ', h * w + 1);
}

Image::~Image() {
    if(bmp) delete[] bmp;
    if(palette) delete[] palette;
}

Image* Image::crop(double crop = 1) {
    Image& img = *this;
    int width = (double)w / crop, height = (double)h / crop;
    char* new_buf = new char[width * height];
    for (int h = 0; h < height; h++)
        for (int w = 0; w < width; w++)
            // new_buf[h * width + w] = *((*this)( (int)( h * crop), (int)( w * crop ) ));
            new_buf[h * width + w] = img( int(h * crop), int(w * crop) );

    delete[] bmp;
    bmp = new_buf;
    return this;
}

Image& Image::scale(double scale = 1) {
    char temp;
    int sum = 0;
    Image& img = *this;
    int width = (double)w / scale, height = (double)h / scale;
    char* new_buf = new char[width * height];
    for (int _h = 0; _h < height; _h++)
        for (int _w = 0; _w < width; _w++) {
            sum = 0;
            for (int __h = _h; __h < int(_h + scale); __h++)
                for (int __w = _w; __w < int(_w + scale); __w++) 
                    sum += img(__h, __w);
            new_buf[_h * w + _w] = palette[sum / (int)scale / (int)scale];
        }
    return *this;
}

std::pair<int,int> Image::resolution() { 
    return std::make_pair(w, h);
}


std::ostream& operator<<(std::ostream& out, Image& img) {
    img.bmp[img.w*img.h] = '\0'; 
    out << img.bmp;
    return out;
}
std::ostream& operator<<(std::ostream& out, Image* img) {
    for(int i = 0; i < img->h; i++) {
        fwrite(&(*img)(i), img->w, 1, stdout); 
        putchar('\n');
    }
    return out;
}