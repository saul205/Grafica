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
    tm.Filmic(imagen);
    //tm.gammaCurveAndClamping(imagen, imagen.getMaximo() * 0.1, 1 / 2.2f);
    // tm.equalization(imagen);

    std::cout << "Introduce una archivo destino: " << std::endl;
    std::cin >> file2;

    escribir(file2, imagen, 255);

    return 0;
}