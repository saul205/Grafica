#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "..\lib\Image.h"

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
            }
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
        Image imagen;
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

int main(){

    std::string file;
    std::cout << "Introduce una imagen: " << std::endl;
    std::cin >> file;
    Image imagen = leer(file);

    std::cout << imagen.toString() << std::endl;

    return 0;
}