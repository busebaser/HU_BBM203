#include "SecretImage.h"


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {

    GrayscaleImage originalImage = image;
    width = image.get_width();
    height = image.get_height();

    int upper_size = (width * (width + 1)) / 2;
    int lower_size = (width * (width - 1)) / 2;


    upper_triangular = new int[upper_size];
    lower_triangular = new int[lower_size];

    int lower_count = 0;

    while(lower_count < lower_size) {

        for(int i = 0; i < width; i++) {
            for(int j = 0; j < i; j++) {
                lower_triangular[lower_count] = image.get_data()[i][j];
                lower_count++;
            }
        }
    }

    int upper_count = upper_size-1;
    while(upper_count >= 0) {
        for(int i = width-1; i >= 0; i--) {
            for(int j = width-1; j >= i; j--) {
                upper_triangular[upper_count] = image.get_data()[i][j];
                upper_count--;
            }
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    // 1. Width ve height ayarlanıyor
    width = w;
    height = h;


    int upper_size = (width * (width + 1)) / 2;
    int lower_size = (width * (width - 1)) / 2;

    upper_triangular = new int[upper_size];
    lower_triangular = new int[lower_size];

    std::copy(upper, upper + upper_size, upper_triangular);
    std::copy(lower, lower + lower_size, lower_triangular);

}


// Destructor: free the arrays
SecretImage::~SecretImage() {
    delete[] upper_triangular; // Bellek serbest bırakma
    delete[] lower_triangular;
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);

    int upper_index = 0;
    int lower_index = 0;


    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i <= j) {
                image.set_pixel(i, j, upper_triangular[upper_index++]);
            } else {
                image.set_pixel(i, j, lower_triangular[lower_index++]);
            }
        }
    }

    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    int upper_index = 0;
    int lower_index = 0;


    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int pixel_value = image.get_pixel(i, j);

            if (i <= j) {
                upper_triangular[upper_index++] = pixel_value;
            } else {
                lower_triangular[lower_index++] = pixel_value;
            }
        }
    }
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    std::ofstream file(filename);
    int upper_size = (width * (width + 1)) / 2;
    int lower_size = (width * (width - 1)) / 2;

    file << width << " " << height << "\n";
    for (int i = 0; i < upper_size; ++i) {
        file << upper_triangular[i];
        if (i != upper_size - 1) {
            file << " ";
        }
    }
    file << "\n";

    for (int i = 0; i < lower_size; ++i) {
        file << lower_triangular[i];
        if (i != lower_size - 1) {
            file << " ";
        }
    }

    file << "\n";
}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {

    std::ifstream file(filename);
    int width, height;
    file >> width >> height;
    int upper_size = (width * (width + 1)) / 2;
    int lower_size = (width * (width - 1)) / 2;
    int* upper_triangular = new int[upper_size];
    int* lower_triangular = new int[lower_size];
    for (int i = 0; i < upper_size; ++i) {
        file >> upper_triangular[i];
    }
    for (int i = 0; i < lower_size; ++i) {
        file >> lower_triangular[i];
    }
    file.close();

    SecretImage secret_image(width, height, upper_triangular, lower_triangular);
    delete[] upper_triangular;
    delete[] lower_triangular;
    return secret_image;
}

// Returns a pointer to the upper triangular part of the secret image.
int * SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int * SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
