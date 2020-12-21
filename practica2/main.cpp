#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "../lib/Image.h"
#include "../lib/ToneMapper.h"


int main(int argc, char** argv){

    std::string file, file2;
    std::cout << "Introduce una imagen: " << std::endl;
    std::cin >> file;
    Image imagen = leer(file);

    ToneMapper tm;
    tm.Filmic(imagen);

    std::cout << "Introduce una archivo destino: " << std::endl;
    std::cin >> file2;

    escribir(file2, imagen, 255);

    return 0;
}