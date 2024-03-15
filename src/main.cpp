
#include "header.h"
using namespace std;


int main() {

    // task 1

    TGAImage layer1 = readTGA("input/layer1.tga");
    TGAImage pattern1 = readTGA("input/pattern1.tga");
    TGAImage Result = multiply(layer1, pattern1);
    writeTGA("output/part1.tga", Result);

    // task 2
    TGAImage layer2 = readTGA("input/layer2.tga");
    TGAImage car = readTGA("input/car.tga");
    TGAImage Result2 = subtract(car, layer2);
    writeTGA("output/part2.tga", Result2);

    //task 3
    TGAImage pattern2 = readTGA("input/pattern2.tga");
    TGAImage text = readTGA("input/text.tga");
    TGAImage Result3 = multiply(layer1, pattern2);
    TGAImage Result_3 = screen(Result3, text);
    writeTGA("output/part3.tga", Result_3);

    //task 4
    TGAImage circles = readTGA("input/circles.tga");
    TGAImage result4 = multiply(circles, layer2);
    TGAImage result_4 = subtract(result4, pattern2);
    writeTGA("output/part4.tga", result_4);

    //task 5
    TGAImage result5 = Overlay(layer1, pattern1);
    writeTGA("output/part5.tga", result5);

    //task 6
    TGAImage result6 = adding(car, 200);
    writeTGA("output/part6.tga", result6);

    //task 7
    TGAImage result7 = task7(car);
    writeTGA("output/part7.tga", result7);

    //task 8
    TGAImage blue, green, red;
    seperate(car, blue, green, red);

    //task 9
    TGAImage layer_blue = readTGA("input/layer_blue.tga");
    TGAImage layer_green = readTGA("input/layer_green.tga");
    TGAImage layer_red = readTGA("input/layer_red.tga");
    TGAImage CombinedImage = combine(layer_blue, layer_green, layer_red);
    writeTGA("output/part9.tga", CombinedImage);

    //task 10
    TGAImage text2 = readTGA("input/text2.tga");
    TGAImage result10 = flip(text2);
    writeTGA("output/part10.tga", result10);


    return 0;
}
