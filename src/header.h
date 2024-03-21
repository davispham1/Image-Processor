#pragma once
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

struct Pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

struct Header {
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
};

struct TGAImage {
    Header header;
    vector<vector<Pixel>> pixelData;
};

// read the header and pixel datas
TGAImage readTGA(const string& filename) {
    ifstream file(filename, ios::binary);
    // if(file.is_open()){
    //   cout << "file is open" << endl;
    // }
    // else{
    //   cout << "file is not open" << endl;

    Header header;
    file.read(reinterpret_cast<char*>(&header.idLength), 1);
    file.read(reinterpret_cast<char*>(&header.colorMapType), 1);
    file.read(reinterpret_cast<char*>(&header.dataTypeCode), 1);
    file.read(reinterpret_cast<char*>(&header.colorMapOrigin), 2);
    file.read(reinterpret_cast<char*>(&header.colorMapLength), 2);
    file.read(reinterpret_cast<char*>(&header.colorMapDepth), 1);
    file.read(reinterpret_cast<char*>(&header.xOrigin), 2);
    file.read(reinterpret_cast<char*>(&header.yOrigin), 2);
    file.read(reinterpret_cast<char*>(&header.width), 2);
    file.read(reinterpret_cast<char*>(&header.height), 2);
    file.read(reinterpret_cast<char*>(&header.bitsPerPixel), 1);
    file.read(reinterpret_cast<char*>(&header.imageDescriptor), 1);

    // image data BGR
    vector<vector<Pixel>> imageData(header.height, vector<Pixel>(header.width));
    for (int i = 0; i < header.height; i++) {
        for (int j = 0; j < header.width; j++) {
            Pixel pixel;
            file.read(reinterpret_cast<char*>(&pixel.blue), 1);
            file.read(reinterpret_cast<char*>(&pixel.green), 1);
            file.read(reinterpret_cast<char*>(&pixel.red), 1);
            imageData[i][j] = pixel;
        }
    }



    file.close();
    return {header, imageData};
}

void writeTGA(const string& filename, const TGAImage& image){
    ofstream file(filename, ios::binary);
    file.write(reinterpret_cast<const char*>(&image.header.idLength), 1);
    file.write(reinterpret_cast<const char*>(&image.header.colorMapType), 1);
    file.write(reinterpret_cast<const char*>(&image.header.dataTypeCode), 1);
    file.write(reinterpret_cast<const char*>(&image.header.colorMapOrigin), 2);
    file.write(reinterpret_cast<const char*>(&image.header.colorMapLength), 2);
    file.write(reinterpret_cast<const char*>(&image.header.colorMapDepth), 1);
    file.write(reinterpret_cast<const char*>(&image.header.xOrigin), 2);
    file.write(reinterpret_cast<const char*>(&image.header.yOrigin), 2);
    file.write(reinterpret_cast<const char*>(&image.header.width), 2);
    file.write(reinterpret_cast<const char*>(&image.header.height), 2);
    file.write(reinterpret_cast<const char*>(&image.header.bitsPerPixel), 1);
    file.write(reinterpret_cast<const char*>(&image.header.imageDescriptor), 1);

    // write the pixels
    for (int i = 0; i < image.header.height; ++i) {
        for (int j = 0; j < image.header.width; ++j) {
            const Pixel& pixel = image.pixelData[i][j];
            file.write(reinterpret_cast<const char*>(&pixel.blue), 1);
            file.write(reinterpret_cast<const char*>(&pixel.green), 1);
            file.write(reinterpret_cast<const char*>(&pixel.red), 1);
        }
    }

    file.close();
}

TGAImage multiply(const TGAImage& topLayer, const TGAImage& bottomLayer) {
    TGAImage result;
    result.header = topLayer.header;
    //pixel data vector
    result.pixelData = vector<vector<Pixel>>(topLayer.header.height, vector<Pixel>(topLayer.header.width));

    for (int i = 0; i < topLayer.header.height; i++) {
        for (int j = 0; j < topLayer.header.width; j++) {
            float blue_Top = topLayer.pixelData[i][j].blue / 255.0f;
            float green_Top = topLayer.pixelData[i][j].green / 255.0f;
            float red_Top = topLayer.pixelData[i][j].red / 255.0f;

            float blue_Bot = bottomLayer.pixelData[i][j].blue / 255.0f;
            float green_Bot = bottomLayer.pixelData[i][j].green / 255.0f;
            float red_Bot = bottomLayer.pixelData[i][j].red / 255.0f;

            result.pixelData[i][j] = {
                    static_cast<unsigned char>(blue_Top * blue_Bot * 255.0f + 0.5f),
                    static_cast<unsigned char>(green_Top * green_Bot * 255.0f + 0.5f),
                    static_cast<unsigned char>(red_Top * red_Bot * 255.0f + 0.5f)
            };
        }
    }
    return result;
}

TGAImage subtract(const TGAImage& topLayer, const TGAImage& bottomLayer){
    TGAImage result;
    result.header = topLayer.header;

    //pixel data vector
    result.pixelData = vector<vector<Pixel>>(topLayer.header.height, vector<Pixel>(topLayer.header.width));

    for (int i = 0; i < topLayer.header.height; i++) {
        for (int j = 0; j < topLayer.header.width; j++) {

            int blue_Top = topLayer.pixelData[i][j].blue;
            int green_Top = topLayer.pixelData[i][j].green;
            int red_Top = topLayer.pixelData[i][j].red;

            int blue_Bot = bottomLayer.pixelData[i][j].blue;
            int green_Bot = bottomLayer.pixelData[i][j].green;
            int red_Bot = bottomLayer.pixelData[i][j].red;

            // Apply Subtract blending
            int blue_Result = blue_Top - blue_Bot;
            int green_Result = green_Top - green_Bot;
            int red_Result = red_Top - red_Bot;

            if (blue_Result < 0) blue_Result = 0;
            if (blue_Result > 255) blue_Result = 255;
            if (green_Result < 0) green_Result = 0;
            if (green_Result > 255) green_Result = 255;
            if (red_Result < 0) red_Result = 0;
            if (red_Result > 255) red_Result = 255;

            result.pixelData[i][j] = {
                    static_cast<unsigned char>(blue_Result),
                    static_cast<unsigned char>(green_Result),
                    static_cast<unsigned char>(red_Result)
            };
        }
    }
    return result;
}

TGAImage add(const TGAImage& topLayer, const TGAImage& bottomLayer){
    TGAImage result;
    result.header = topLayer.header;

    //pixel data vector
    result.pixelData = vector<vector<Pixel>>(topLayer.header.height, vector<Pixel>(topLayer.header.width));

    for (int i = 0; i < topLayer.header.height; i++) {
        for (int j = 0; j < topLayer.header.width; j++) {

            int blue_Top = topLayer.pixelData[i][j].blue;
            int green_Top = topLayer.pixelData[i][j].green;
            int red_Top = topLayer.pixelData[i][j].red;

            int blue_Bot = bottomLayer.pixelData[i][j].blue;
            int green_Bot = bottomLayer.pixelData[i][j].green;
            int red_Bot = bottomLayer.pixelData[i][j].red;

            int blue_Result = blue_Top + blue_Bot;
            int green_Result = green_Top + green_Bot;
            int red_Result = red_Top + red_Bot;

            if (blue_Result < 0) blue_Result = 0;
            if (blue_Result > 255) blue_Result = 255;
            if (green_Result < 0) green_Result = 0;
            if (green_Result > 255) green_Result = 255;
            if (red_Result < 0) red_Result = 0;
            if (red_Result > 255) red_Result = 255;

            result.pixelData[i][j] = {
                    static_cast<unsigned char>(blue_Result),
                    static_cast<unsigned char>(green_Result),
                    static_cast<unsigned char>(red_Result)
            };
        }
    }
    return result;
}

TGAImage screen(const TGAImage& topLayer, const TGAImage& bottomLayer) {
    TGAImage result;
    result.header = topLayer.header;
    result.pixelData = vector<vector<Pixel>>(topLayer.header.height, vector<Pixel>(topLayer.header.width));

    for (int i = 0; i < topLayer.header.height; i++) {
        for (int j = 0; j < topLayer.header.width; j++) {
            float blue_Top = topLayer.pixelData[i][j].blue / 255.0f;
            float green_Top = topLayer.pixelData[i][j].green / 255.0f;
            float red_Top = topLayer.pixelData[i][j].red / 255.0f;

            float blue_Bot = bottomLayer.pixelData[i][j].blue / 255.0f;
            float green_Bot = bottomLayer.pixelData[i][j].green / 255.0f;
            float red_Bot = bottomLayer.pixelData[i][j].red / 255.0f;

            result.pixelData[i][j] = {
                    static_cast<unsigned char>((1.0f - (1.0f - blue_Top) * (1.0f - blue_Bot)) * 255.0f + 0.5f),
                    static_cast<unsigned char>((1.0f - (1.0f - green_Top) * (1.0f - green_Bot)) * 255.0f + 0.5f),
                    static_cast<unsigned char>((1.0f - (1.0f - red_Top) * (1.0f - red_Bot)) * 255.0f + 0.5f)
            };
        }
    }
    return result;
}

TGAImage Overlay(const TGAImage& topLayer, const TGAImage& bottomLayer) {
    TGAImage result;
    result.header = topLayer.header;
    result.pixelData = vector<vector<Pixel>>(topLayer.header.height, vector<Pixel>(topLayer.header.width));

    for (int i = 0; i < topLayer.header.height; ++i) {
        for (int j = 0; j < topLayer.header.width; ++j) {
            float blue_Top = topLayer.pixelData[i][j].blue / 255.0f;
            float green_Top = topLayer.pixelData[i][j].green / 255.0f;
            float red_Top = topLayer.pixelData[i][j].red / 255.0f;

            float blue_Bot = bottomLayer.pixelData[i][j].blue / 255.0f;
            float green_Bot = bottomLayer.pixelData[i][j].green / 255.0f;
            float red_Bot = bottomLayer.pixelData[i][j].red / 255.0f;

            float blue_Result, green_Result, red_Result;

            if (blue_Bot <= 0.5f)
                blue_Result = 2.0f * blue_Top * blue_Bot;
            else
                blue_Result = 1.0f - 2.0f * (1.0f - blue_Top) * (1.0f - blue_Bot);
            if (green_Bot <= 0.5f)
                green_Result = 2.0f * green_Top * green_Bot;
            else
                green_Result = 1.0f - 2.0f * (1.0f - green_Top) * (1.0f - green_Bot);
            if (red_Bot <= 0.5f)
                red_Result = 2.0f * red_Top * red_Bot;
            else
                red_Result = 1.0f - 2.0f * (1.0f - red_Top) * (1.0f - red_Bot);
            result.pixelData[i][j] = {
                    static_cast<unsigned char>(blue_Result *255.0f + 0.5f),
                    static_cast<unsigned char>(green_Result * 255.0f + 0.5f),
                    static_cast<unsigned char>(red_Result * 255.0f + 0.5f)
            };
        }
    }
    return result;
}

TGAImage adding(const TGAImage& image, int add, char channel) {
    TGAImage result;
    result.header = image.header;
    result.pixelData = vector<vector<Pixel>>(image.header.height, vector<Pixel>(image.header.width));

    for (int i = 0; i < image.header.height; i++) {
        for (int j = 0; j < image.header.width; j++) {
            unsigned char red = image.pixelData[i][j].red;
            unsigned char green = image.pixelData[i][j].green;
            unsigned char blue = image.pixelData[i][j].blue;
            if (channel == 'r'){
                red = image.pixelData[i][j].red + add;
                if (red < 0) red = 0;
                if (red > 255) red = 255;
            }
            else if (channel == 'g'){
                green = image.pixelData[i][j].green + add;
                if (green < 0) green = 0;
                if (green > 255) green = 255;
            }
            else if (channel == 'b'){
                blue = image.pixelData[i][j].blue + add;
                if (blue < 0) blue = 0;
                if (blue > 255) blue = 255;
            }
            result.pixelData[i][j] = {blue, green, red};
        }
    }
    return result;
}

TGAImage scaling(const TGAImage& image, int scale, char channel) {
    TGAImage result;
    result.header = image.header;
    result.pixelData = vector<vector<Pixel>>(image.header.height, vector<Pixel>(image.header.width));

    for (int i = 0; i < image.header.height; i++) {
        for (int j = 0; j < image.header.width; j++) {
            unsigned char red = image.pixelData[i][j].red;
            unsigned char green = image.pixelData[i][j].green;
            unsigned char blue = image.pixelData[i][j].blue;

            if (channel == 'r') {
                red *= scale;
                if (red < 0) red = 0;
                if (red > 255) red = 255;
            } else if (channel == 'g') {
                green *= scale;
                if (green < 0) green = 0;
                if (green > 255) green = 255;
            } else if (channel == 'b') {
                blue *= scale;
                if (blue < 0) blue = 0;
                if (blue > 255) blue = 255;
            }

            result.pixelData[i][j] = {blue, green, red};
        }
    }
    return result;
}

void seperate(const TGAImage& image, TGAImage& blue, TGAImage& green, TGAImage& red){
    blue.header = image.header;
    green.header = image.header;
    red.header = image.header;
    blue.pixelData = vector<vector<Pixel>>(image.header.height, vector<Pixel>(image.header.width));
    green.pixelData = vector<vector<Pixel>>(image.header.height, vector<Pixel>(image.header.width));
    red.pixelData = vector<vector<Pixel>>(image.header.height, vector<Pixel>(image.header.width));

    for (int i = 0; i < image.header.height; i++) {
        for (int j = 0; j < image.header.width; j++) {
            blue.pixelData[i][j] = {image.pixelData[i][j].blue, image.pixelData[i][j].blue, image.pixelData[i][j].blue};
            green.pixelData[i][j] = {image.pixelData[i][j].green, image.pixelData[i][j].green, image.pixelData[i][j].green};
            red.pixelData[i][j] = {image.pixelData[i][j].red, image.pixelData[i][j].red, image.pixelData[i][j].red};
        }
    }
    writeTGA("output/part8_b.tga", blue);
    writeTGA("output/part8_g.tga", green);
    writeTGA("output/part8_r.tga", red);
}

TGAImage only(const TGAImage& image, TGAImage& channel, char color) {
    channel.header = image.header;
    channel.pixelData = vector<vector<Pixel>>(image.header.height, vector<Pixel>(image.header.width));
    for (int i = 0; i < image.header.height; i++) {
        for (int j = 0; j < image.header.width; j++) {
            if (color == 'r') {
                channel.pixelData[i][j] = {image.pixelData[i][j].red, image.pixelData[i][j].red,
                                           image.pixelData[i][j].red};
            }
            else if (color == 'g') {
                channel.pixelData[i][j] = {image.pixelData[i][j].green, image.pixelData[i][j].green,
                                           image.pixelData[i][j].green};
            }
            else if (color == 'b') {
                channel.pixelData[i][j] = {image.pixelData[i][j].blue, image.pixelData[i][j].blue,
                                           image.pixelData[i][j].blue};
            }
        }
    }
    return channel;
}

TGAImage combine(const TGAImage& blue, const TGAImage& green, const TGAImage& red) {
    TGAImage result;
    result.header = blue.header;
    result.pixelData = vector<vector<Pixel>>(blue.header.height, vector<Pixel>(blue.header.width));

    for (int i = 0; i < blue.header.height; i++) {
        for (int j = 0; j < blue.header.width; j++) {
            result.pixelData[i][j] = {
                    blue.pixelData[i][j].blue,
                    green.pixelData[i][j].green,
                    red.pixelData[i][j].red
            };
        }
    }
    return result;
}

TGAImage flip(const TGAImage& image){
    TGAImage result;
    result.header = image.header;
    result.pixelData = vector<vector<Pixel>>(image.header.height, vector<Pixel>(image.header.width));

    for (int i = 0; i < image.header.height; i++) {
        for (int j = 0; j < image.header.width; j++) {
            result.pixelData[image.header.height - 1 - i][image.header.width - 1 - j] = image.pixelData[i][j];
        }
    }
    return result;
}

bool isValidFileName(const string& filename) {
    return filename.size() >= 4 && filename.substr(filename.size() - 4) == ".tga";
}

bool checkArguments(int argc, char* argv[], int index) {
    if (index >= argc) {
        cout << "Missing argument." << endl;
        return false;
    }
    return true;
}

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.is_open();
}
