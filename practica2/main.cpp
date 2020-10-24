#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "../lib/Image.h"
#include "../lib/ToneMapper.h"


int main(){

    std::string file, file2;
    std::cout << "Introduce una imagen: " << std::endl;
    std::cin >> file;
    Image imagen = leer(file);

    ToneMapper tm;
    tm.Reinhard(imagen, 1);

    std::cout << "Introduce una archivo destino: " << std::endl;
    std::cin >> file2;

    escribirbmp(file2, imagen, 255);

    return 0;
}