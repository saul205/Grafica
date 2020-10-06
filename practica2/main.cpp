#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "../lib/Image.h"
#include "../lib/ToneMapper.h"

Image leer(std::string nombre){

    std::ifstream f(nombre);
    std::string format;
    std::string linea;
    int numDatos = 0;

    int width, height, color_res;
    float maximo = 1.0f;
    bool maxFound = false; // True si se ha encontrado el comentario MAX

    if(f.is_open()){
        getline(f, format);

        while(numDatos != 2){
            getline(f, linea);
            if(linea[0] != '#'){                // Altura y anchura
                if(numDatos == 0){
                    std::istringstream iss(linea);
                    iss >> width;
                    iss >> height;
                    ++numDatos;
                } else {                        // Color resolution
                    color_res = stoi(linea);
                    ++numDatos;
                }
            } else {
                if(linea.substr(0,5) == "#MAX="){
                    linea = linea.substr(5);
                    maximo = stof(linea);
                    maxFound = true;
                }
            }std::ifstream f(nombre);
        }

        // Lee el resto de comentarios que puedan existir
        int firstDigit;
        firstDigit = f.peek();
        while(!std::isdigit(firstDigit)){
            getline(f, linea);
            if(!maxFound && linea.substr(0,5) == "#MAX="){
                linea = linea.substr(5);
                maximo = stof(linea);
                maxFound = true;
            }
        }

        int elementCounter = 0;
        Image imagen(format, nombre, width, height, color_res, maximo);
        float rgbR = 0, rgbG = 0, rgbB = 0;
        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                f >> rgbR;
                f >> rgbG;
                f >> rgbB;
                imagen.set(elementCounter, rgbR, rgbG, rgbB);
                ++elementCounter;
            }
        }
        return imagen;
    }
    else{
        std::cout << "Error en la lectura" << std::endl;
        return Image();
    }
    
}

void escribir(std::string nombre, Image& img){

    std::ofstream f(nombre);

    if(f.is_open()){

        std::string write = "";
        write += img.getCabecera() + "\n";
        write += "#MAX=" + std::to_string(img.getMax()) + "\n";
        write += std::to_string(img.getWidth()) + " " + std::to_string(img.getHeight()) + "\n";
        write += std::to_string(255) + "\n";
        f << write;
        
        for(int i = 0; i < img.getHeight(); ++i){
            f << img.toStringRow(i) << "\n";
        }
    }
}

int main(){

    std::string file, file2;
    std::cout << "Introduce una imagen: " << std::endl;
    std::cin >> file;
    Image imagen = leer(file);

    std::cout << imagen.getMax() << endl;

    ToneMapper tm;
    tm.clamping(imagen);

    std::cout << "Introduce una archivo destino: " << std::endl;
    std::cin >> file2;
    escribir(file2, imagen);

    return 0;
}