#include <iostream>
#include "header.h"
#include <fstream>
#include <cstring>
using namespace std;


int main(int argc, char* argv[]) {

    if (argc == 1 || strcmp(argv[1], "--help") == 0) {
        cout << "Project 2: Image Processing, Spring 2024\n" << endl;
        cout << "Usage:\n"
                  << "\t./project2.out [output] [firstImage] [method] [...]" << std::endl;
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


    TGAImage result = readTGA(inputfile);

    int index = 3;

    for (int i = 3; i < argc; i++) {
        string operation = argv[i];

        if (operation == "multiply") {
            if (!checkArguments(argc, argv, i + 1)) return 1;
            string inputfile2 = argv[++i];
            if (!isValidFileName(inputfile2)) {
                cout << "Invalid argument, invalid file name." << endl;
                return 1;
            }
            if(!fileExists(inputfile2)){
                cout << "Invalid argument, file does not exist." << endl;
                return 1;
            }

            TGAImage image2 = readTGA(inputfile2);
            result = multiply(result, image2);
        }
        else if (operation == "subtract") {
            if (!checkArguments(argc, argv, i + 1)) return 1;
            string inputfile2 = argv[++i];
            if (!isValidFileName(inputfile2)) {
                cout << "Invalid argument, invalid file name." << endl;
                return 1;
            }
            if(!fileExists(inputfile2)){
                cout << "Invalid argument, file does not exist." << endl;
                return 1;
            }
            TGAImage image2 = readTGA(inputfile2);
            result = subtract(result, image2);
        }
        else if (operation == "overlay") {
            if (!checkArguments(argc, argv, i + 1)) return 1;
            string inputfile2 = argv[++i];
            if (!isValidFileName(inputfile2)) {
                cout << "Invalid argument, invalid file name." << endl;
                return 1;
            }
            if(!fileExists(inputfile2)){
                cout << "Invalid argument, file does not exist." << endl;
                return 1;
            }
            TGAImage image2 = readTGA(inputfile2);
            result = Overlay(result, image2);
        }
        else if (operation == "screen") {
            if (!checkArguments(argc, argv, i + 2)) return 1;
            string inputfile2 = argv[++i];
            if (!isValidFileName(inputfile2)) {
                cout << "Invalid argument, invalid file name." << endl;
                return 1;
            }
            if(!fileExists(inputfile2)){
                cout << "Invalid argument, file does not exist." << endl;
                return 1;
            }
            TGAImage image2 = readTGA(inputfile2);
            result = screen(image2, result);
        }
        else if (operation == "combine") {
            if (!checkArguments(argc, argv, i + 2)) return 1;
            string inputfile2 = argv[++i];
            if (!isValidFileName(inputfile2)) {
                cout << "Invalid argument, invalid file name." << endl;
                return 1;
            }
            if(!fileExists(inputfile2)){
                cout << "Invalid argument, file does not exist." << endl;
                return 1;
            }
            TGAImage image2 = readTGA(inputfile2);
            string inputfile3 = argv[++i];
            TGAImage blue = readTGA(inputfile3);
            result = combine(blue, image2, result);
        }
        else if (operation == "flip") {
            result = flip(result);
            while (i + 1 < argc && string(argv[i + 1]) == "flip") {
                result = flip(result);
                i++;
            }
        }
        else if (operation == "onlyred") {
                TGAImage red;
                result = only(result, red, 'r');
            }
        else if (operation == "onlygreen") {
                TGAImage green;
                result = only(result, green, 'g');
            }
        else if (operation == "onlyblue") {
                TGAImage blue;
                result = only(result, blue, 'b');
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
                result = adding(result, value, 'r');
            }
            else if (operation == "addgreen") {
                result = adding(result, value, 'g');
            }
            else if (operation == "addblue") {
                result = adding(result, value, 'b');
            }
            else if (operation == "scalered") {
                result = scaling(result, value, 'r');
            }
            else if (operation == "scalegreen") {
                result = scaling(result, value, 'g');
            }
            else if (operation == "scaleblue") {
                result = scaling(result, value, 'b');
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

