#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>


// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {

    GrayscaleImage originalImage = image;
    int n = kernelSize/2;

    for (int y = 0; y < image.get_height() ; ++y) {
        for (int x = 0; x < image.get_width() ; ++x) {
            int sum = 0;
            int count = 0;

            for (int ky = -n; ky <= n; ++ky) {
                for (int kx = -n; kx <= n; ++kx) {
                    int present_x = x + kx;
                    int present_y = y + ky;


                    if (present_x >= 0 && present_x < image.get_width() && present_y >= 0 && present_y < image.get_height()) {
                        sum += originalImage.get_pixel(present_x, present_y);
                    }
                    count++;
                }
            }

            int value =  sum / count;
            image.set_pixel(x, y, value);
        }
    }
}



// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {

    GrayscaleImage originalImage = image;
    double r,s = 2.0 * sigma * sigma;
    double sum = 0.0;
    int n = kernelSize/2;
    auto** kernel = new double*[kernelSize];
    for (int i = 0; i < kernelSize; i++) {
        kernel[i] = new double[kernelSize];
    }

    for(int x= -n; x <= n; x++) {
        for(int y = -n; y<=n; y++) {
            r = sqrt(x*x + y*y);
            kernel[x+n][y+n] = (exp(-(r*r)/s)) / (3.14159265358979323846 * s);
            sum += kernel[x+n][y+n];
        }
    }
    for(int i=0; i < kernelSize;++i) {
        for(int j=0; j<kernelSize; ++j) {
            kernel[i][j] /= sum;
        }
    }

    for (int y = 0; y < image.get_height() ; ++y) {
        for (int x = 0; x < image.get_width() ; ++x) {
            double sum_result = 0.0;


            for (int ky = -n; ky <= n; ++ky) {
                for (int kx = -n; kx <= n; ++kx) {
                    int present_x = x + kx;
                    int present_y = y + ky;

                    if (present_x >= 0 && present_x < image.get_width() && present_y >= 0 && present_y < image.get_height()) {
                        sum_result += originalImage.get_pixel(present_x, present_y)*kernel[kx + n][ky + n];
                    }
                }
            }

            int value = static_cast<int>(sum_result);
            image.set_pixel(x, y, value);
        }
    }

    for (int i = 0; i < kernelSize; i++) {
        delete[] kernel[i];
    }
    delete[] kernel;
}


// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    GrayscaleImage originalImage = image;
    GrayscaleImage blurredImage = image;

    apply_gaussian_smoothing(blurredImage, kernelSize, 1.0);

    for(int i = 0; i < image.get_height(); ++i) {
        for(int j = 0; j < image.get_width(); ++j) {

            int edge = originalImage.get_data()[i][j] - blurredImage.get_data()[i][j]  ;

            int sharpened_value = static_cast<int>(originalImage.get_data()[i][j] + edge * amount);


            if (sharpened_value > 255) {
                sharpened_value = 255;
            }
            else if (sharpened_value < 0) {
                sharpened_value = 0;
            }

            image.set_pixel(i, j, sharpened_value);
        }
    }
}
