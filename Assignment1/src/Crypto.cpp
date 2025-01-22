#include "Crypto.h"
#include "GrayscaleImage.h"
#include <bitset>


std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;

    GrayscaleImage grayscale_image = secret_image.reconstruct();
    int grayscaleImage_dimensions = grayscale_image.get_height() * grayscale_image.get_width();
    int bitstream_length = message_length * 7;

    if (grayscaleImage_dimensions < bitstream_length) {
        throw std::runtime_error("The image does not have enough pixels.");
    }

    int starting_pixel_x = (grayscaleImage_dimensions - bitstream_length) / grayscale_image.get_width();
    int starting_pixel_y = (grayscaleImage_dimensions - bitstream_length) % grayscale_image.get_width();

    for (int i = starting_pixel_x; i < grayscale_image.get_height(); i++) {
        for (int j = (i == starting_pixel_x ? starting_pixel_y : 0); j < grayscale_image.get_width(); j++) {
            if (LSB_array.size() >= bitstream_length) {
                return LSB_array;  // Stop if we've collected enough bits
            }
            int lastBit = grayscale_image.get_data()[i][j] & 1;
            LSB_array.push_back(lastBit);
        }
    }
    return LSB_array;
}



// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;

    if(LSB_array.size() % 7 != 0) {
        throw std::runtime_error("The size of LSB_array is incorrect. LSB array must be multiple of 7!");
    }

    for (int i = 0; i < LSB_array.size(); i += 7) {
        std::bitset<7> bitstream;

        for (int j = 0; j < 7 && (i + j) < LSB_array.size(); j++) {
            bitstream[6 - j] = LSB_array[i + j]; // bitleri ters sırada ekle
        }


        char ascii_char = static_cast<char>(bitstream.to_ulong());


        message += ascii_char;
    }

    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;
    for (char character : message) {
        std::bitset<7> bitstream(character);


        for (int i = 6; i >= 0; i--) {
            LSB_array.push_back(bitstream[i]);
        }
    }
    return LSB_array;
}

SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {
    GrayscaleImage grayscale_image = image;
    int grayscale_image_dimension = grayscale_image.get_height() * grayscale_image.get_width();
    int bitstream_length = static_cast<int>(LSB_array.size());

    if (bitstream_length > grayscale_image_dimension) {
        throw std::runtime_error("The size of LSB_array is larger than the dimension of the Grayscale Image.");
    }

    int starting_pixel_x = (grayscale_image_dimension - bitstream_length) / grayscale_image.get_width();
    int starting_pixel_y = (grayscale_image_dimension - bitstream_length) % grayscale_image.get_width();

    int bit_counter = 0;


    for (int i = starting_pixel_x; i < grayscale_image.get_height(); i++) {
        for (int j = (i == starting_pixel_x ? starting_pixel_y : 0); j < grayscale_image.get_width(); j++) {
            if (bit_counter >= bitstream_length) {
                break;
            }

            int pixel_value = grayscale_image.get_pixel(i, j);
            int lastBit = pixel_value & 1;

            if (lastBit != LSB_array[bit_counter]) {
                int changed_pixel_value = (pixel_value & ~1) | LSB_array[bit_counter];
                grayscale_image.set_pixel(i, j, changed_pixel_value);
            }
            bit_counter++;
        }

        if (bit_counter >= bitstream_length) {
            break;
        }
    }

    SecretImage secret_image(grayscale_image);
    return secret_image;
}

