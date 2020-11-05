#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Rgb.h"

class Image{
    
public:

    Image(){
    }

    Image(std::string _cabecera, std::string _name, int _width, int _height, int _c, float _max){
        name = _name;
        width = _width;
        height = _height;
        c = _c;
        max = _max;
        cabecera = _cabecera;

        data.resize(width * height);
    }

    void setRGB(int index, rgb colors){
        data[index] = colors;
    }

    void set(int index, int r, int g, int b){
        data[index] = rgb(r * max / c, g * max/ c, b * max/ c);
    }

    rgb get(int index){
        return data[index];
    }

    rgb getRGB(int h, int w){
        return data[h*width + w];
    }

    int getWidth(){
        return width;
    }

    std::string getCabecera(){
        return cabecera;
    }

    void setCabecera(std::string c){
        cabecera = c;
    }

    int getHeight(){
        return height;
    }

    std::string getName(){
        return name;
    }

    float getMaximo(){
        float maximo = get(0).r;
        for(int i = 0; i < getWidth()*getHeight(); ++i){
            rgb colores = get(i);
            if(colores.r > maximo){ maximo = colores.r; }
            if(colores.g > maximo){ maximo = colores.g; }
            if(colores.b > maximo){ maximo = colores.b; }
        }
        return maximo;
    }

    std::string toStringRow(int row, const int col_res, const float new_max){
        std::string ret = "";
        int index = width*row;
        for (int i = 0; i < width; ++i){
            ret += std::to_string((int)(data[index + i].r*(col_res / new_max))) + " ";
            ret += std::to_string((int)(data[index + i].g*(col_res / new_max))) + " ";
            ret += std::to_string((int)(data[index + i].b*(col_res / new_max)));
            ret = (i+1 != width) ? ret + "     " : ret;
        }
        return ret;
    }

private:

    std::string cabecera = "";

    std::vector<rgb> data;
    float max = 1.0;

    std::string name = "";
    int width = 0, height = 0, c = 0;

};


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

void escribir(std::string nombre, Image& img, const int col_res){

    std::ofstream f(nombre);
    if(f.is_open()){

        std::string write = "";
        float maximo = img.getMaximo();

        if(img.getCabecera() != ""){
            write += img.getCabecera() + "\n";
        } else {
            write += "P3\n";
        }

        write += "#MAX=" + std::to_string(maximo) + "\n";
        write += std::to_string(img.getWidth()) + " " + std::to_string(img.getHeight()) + "\n";
        write += std::to_string(col_res) + "\n";
        f << write;
        
        for(int i = 0; i < img.getHeight(); ++i){
            f << img.toStringRow(i, col_res, maximo) << "\n";
        }

        f.close();
    }
}

void escribirbmp(std::string nombre, Image& img, const int col_res){

    std::ofstream f(nombre);
    if(f.is_open()){

        const int fileheadersize = 14;
        const int infoheadersize = 40;

        // EStructura del bitmap
        unsigned char fileheader[fileheadersize] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        unsigned char infoheader[infoheadersize] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        // Indican formato de la imagen
        fileheader[0] = 'B';
        fileheader[1] = 'M';
        // Tamaño fileheader + infoheader en bytes
        fileheader[10] = fileheadersize + infoheadersize;
        // Tamaño infoheader en bytes
        infoheader[0] = infoheadersize;
        // An integer (unsigned) representing the number of color planes of the target device. Should be '1' in decimal.
        infoheader[12] = 1;
        // An integer (unsigned) representing the number of bits (memory) a pixel takes (in pixel data) to represent a color.
        // Es little endian, el segundo byte del campo no nos hará falta
        infoheader[14] = 24;

        // Antes de escribir hay que ajustar el padding, "each scan line is 0-padded to the nearest 4-byte boundary"
        // Al usar 3 bytes por pixel es necesario ajustar el padding;
        int paddingNeeded = (3*img.getWidth()) % 4;
        int filesize = infoheadersize + fileheadersize + 3*img.getWidth()*img.getHeight() + paddingNeeded*img.getHeight();

        // Bytes 3-6 : An integer (unsigned) representing entire file size in bytes. This value is basically 
        // the number of bytes in a BMP image file.
        fileheader[2] = (unsigned char)(filesize);
        fileheader[3] = (unsigned char)(filesize >> 8);
        fileheader[4] = (unsigned char)(filesize >> 16);
        fileheader[5] = (unsigned char)(filesize >> 24);

        // ImageWidth 4 bytes An integer (signed) representing the width of the final image in pixels.
        infoheader[4] = (unsigned char)(img.getWidth());
        infoheader[5] = (unsigned char)(img.getWidth() >> 8);
        infoheader[6] = (unsigned char)(img.getWidth() >> 16);
        infoheader[7] = (unsigned char)(img.getWidth() >> 24);
        // ImageHeigth 4 bytes An integer (signed) representing the height of the final image in pixels.
        infoheader[8] = (unsigned char)(img.getHeight());
        infoheader[9] = (unsigned char)(img.getHeight() >> 8);
        infoheader[10] = (unsigned char)(img.getHeight() >> 16);
        infoheader[11] = (unsigned char)(img.getHeight() >> 24);


        std::string write;
        for(int i = 0; i < fileheadersize; ++i){
            write += fileheader[i];
        }
        for(int i = 0; i < infoheadersize; ++i){
            write += infoheader[i];
        }

        f << write;

        const int h = img.getHeight();
        const int w = img.getWidth();
        const float new_max = img.getMaximo();

        for(int i = h; i > 0; --i){
            write = "";
            for (int j = 0; j < w; ++j){
                write += (char)(img.get(w*(i-1) + j).b*(col_res / new_max));
                write += (char)(img.get(w*(i-1) + j).g*(col_res / new_max));
                write += (char)(img.get(w*(i-1) + j).r*(col_res / new_max));
            }

            // Añadir el padding restante a la línea
            for(int k = 0; k < paddingNeeded; ++k){
                write += '0';
            }

            f << write;
        }

        f.close();
    }
}
#endif