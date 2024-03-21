#include <iostream>
#include "header.h"
#include <fstream>
#include <cstring>
using namespace std;


int main(int argc, char* argv[]) {

    if (argc < 4 || std::strcmp(argv[1], "--help") == 0) {
        std::cout << "Project 2: Image Processing, Fall 2023\n"
                  << "Usage:\n"
                  << "\t./project.out [output] [firstimage] [method] [...]" << std::endl;
        return 0;
    }

    string outputfile = argv[1];

    // check outputfile
    if (!isValidFileName(outputfile)){
        cout << "Invalid file name." << endl;
        return 1;
    }


    string inputfile = argv[2];

    // check inputfile
    if (!isValidFileName(inputfile)) {
        cout << "Invalid file name." << endl;
        return 1;
    }
    // check if exist
    ifstream file(inputfile);
    if (!file.is_open()) {
        cout << "File does not exist." << endl;
        return 1;
    }


    TGAImage image1 = readTGA(inputfile);
    TGAImage result;


    for (int i = 3; i < argc; i++) {
        string operation = argv[i];

        if (operation == "multiply") {
            if (!checkArguments(argc, argv, i + 1)) return 1;
            string inputfile2 = argv[++i];
            if (!isValidFileName(inputfile2)) {
                cout << "Invalid argument, invalid file name." << endl;
                return 1;
            }
            TGAImage image2 = readTGA(inputfile2);
            result = multiply(image1, image2);
        }
        else if (operation == "subtract") {
            if (!checkArguments(argc, argv, i + 1)) return 1;
            string inputfile2 = argv[++i];
            if (!isValidFileName(inputfile2)) {
                cout << "Invalid argument, invalid file name." << endl;
                return 1;
            }
            TGAImage image2 = readTGA(inputfile2);
            result = subtract(image1, image2);
        }
        else if (operation == "overlay") {
            if (!checkArguments(argc, argv, i + 1)) return 1;
            string inputfile2 = argv[++i];
            if (!isValidFileName(inputfile2)) {
                cout << "Invalid argument, invalid file name." << endl;
                return 1;
            }
            TGAImage image2 = readTGA(inputfile2);
            result = Overlay(image1, image2);
        }
        else if (operation == "screen") {
            if (!checkArguments(argc, argv, i + 2)) return 1;
            string inputfile2 = argv[++i];
            if (!isValidFileName(inputfile2)) {
                cout << "Invalid argument, invalid file name." << endl;
                return 1;
            }
            TGAImage image2 = readTGA(inputfile2);
            result = screen(image2, image1);
        }
        else if (operation == "combine") {
            if (!checkArguments(argc, argv, i + 2)) return 1;
            string inputfile2 = argv[i++];
            if (!isValidFileName(inputfile2)) {
                cout << "Invalid argument, invalid file name." << endl;
                return 1;
            }
            TGAImage image2 = readTGA(inputfile2);
            string inputfile3 = argv[i++];
            TGAImage blue = readTGA(inputfile3);
            result = combine(blue, image2, image1);
        }
        else if (operation == "flip") {
            result = flip(image1);
            while (i + 1 < argc && string(argv[i + 1]) == "flip") {
                result = flip(result);
                i++;
            }
        }
        else if (operation == "onlyred") {
                TGAImage red;
                result = only(image1, red, 'r');
            }
        else if (operation == "onlygreen") {
                TGAImage green;
                result = only(image1, green, 'g');
            }
        else if (operation == "onlyblue") {
                TGAImage blue;
                result = only(image1, blue, 'b');
            }
        else if (operation == "addred" || operation == "addgreen" || operation == "addblue" ||
                 operation == "scalered" || operation == "scalegreen" || operation == "scaleblue") {
            if (!checkArguments(argc, argv, i + 1)) return 1;
            int value;
            try {
                value = stoi(argv[i + 1]);
            }
            catch (const invalid_argument&) {
                cout << "Invalid argument, expected number." << endl;
                return 1;
            }
            if (operation == "addred") {
                result = adding(image1, value, 'r');
            }
            else if (operation == "addgreen") {
                result = adding(image1, value, 'g');
            }
            else if (operation == "addblue") {
                result = adding(image1, value, 'b');
            }
            else if (operation == "scalered") {
                result = scaling(image1, value, 'r');
            }
            else if (operation == "scalegreen") {
                result = scaling(image1, value, 'g');
            }
            else if (operation == "scaleblue") {
                result = scaling(image1, value, 'b');
            }
            i++;
        }
        else {
            cout << "Invalid method name." << endl;
            return 1;
        }
    }
    writeTGA(outputfile,result);


    return 0;
}
