#include "GrayscaleImage.h"
#include <iostream>
#include <cstring>  // For memcpy
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdexcept>


GrayscaleImage::GrayscaleImage(const char* filename) {


    int channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_grey);

    if (image == nullptr) {
        std::cerr << "Error: Could not load image " << filename << std::endl;
        exit(1);
    }

    data = new int*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new int[width];
    }


    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            data[y][x] = image[y * width + x];
        }
    }


    stbi_image_free(image);
}

GrayscaleImage::GrayscaleImage(int** inputData, int h, int w) {
    width = w;
    height=h;
    data = new int *[height];
    for(int i = 0; i< height; i++) {
        data[i] = new int[width];
    }
    for(int y = 0; y<height;++y) {
        for(int x=0; x< width;++x) {
            data[y][x] = inputData[y][x];
        }
    }
}

GrayscaleImage::GrayscaleImage(int w, int h) : width(w), height(h) {
    data = new int *[height];

    for(int i=0;i < height;i++) {
        data[i] = new int[width];
    }
}

GrayscaleImage::GrayscaleImage(const GrayscaleImage& other) {
    width = other.width;
    height = other.height;

    data = new int*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new int[width];
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            data[y][x] = other.data[y][x];
        }
    }
}


GrayscaleImage::~GrayscaleImage() {
    for (int i = 0; i < height; ++i) {
        delete[] data[i];
    }

    delete[] data;
}


bool GrayscaleImage::operator==(const GrayscaleImage& other) const {
    if(height!= other.height || width != other.width) {
        return false;
    }

    for(int i=0; i<height;++i){
        for(int j=0; j<width; ++j) {
            if(data[i][j] != other.data[i][j]) {
                return false;
            }
        }
    }
    return true;
}


GrayscaleImage GrayscaleImage::operator+(const GrayscaleImage& other) const {
    GrayscaleImage result(width, height);

    for(int i=0;i<height;++i) {
        for(int j=0; j<width; ++j) {
            int sum = data[i][j] + other.data[i][j];
            if(sum>255){
                sum = 255;
            }
            result.set_pixel(i,j,sum);
        }
    }
    return result;
}


GrayscaleImage GrayscaleImage::operator-(const GrayscaleImage& other) const {
    GrayscaleImage result(width, height);

    for(int i=0;i<height;++i) {
        for(int j=0; j<width; ++j) {
            int sub = data[i][j] - other.data[i][j];
            if(sub<0) {
                sub = 0;
            }
            result.set_pixel(i,j,sub);
        }
    }
    return result;
}

int GrayscaleImage::get_pixel(int row, int col) const {
    return data[row][col];
}

void GrayscaleImage::set_pixel(int row, int col, int value) {
    data[row][col] = value;
}

void GrayscaleImage::save_to_file(const char* filename) const {
    unsigned char* imageBuffer = new unsigned char[width * height];

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            imageBuffer[i * width + j] = static_cast<unsigned char>(data[i][j]);
        }
    }


    if (!stbi_write_png(filename, width, height, 1, imageBuffer, width)) {
        std::cerr << "Error: Could not save image to file " << filename << std::endl;
    }

    delete[] imageBuffer;
}
