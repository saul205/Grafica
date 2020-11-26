#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Rgb.hpp"

class Image{
    
public:

    Image(){
    }

    Image(std::string _cabecera, std::string _name, int _width, int _height, int _c, float _max);

    // ---- GETTERS Y SETTERS ----
    void setRGB(int index, rgb colors);
    void set(int index, int r, int g, int b);

    rgb get(int index);
    rgb getRGB(int h, int w);

    int getWidth();
    int getHeight();
    float getMaximo();
    
    void setCabecera(std::string c);
    std::string getCabecera();

    std::string getName();
    
    // toString() de una úncia fila
    std::string toStringRow(int row, const int col_res, const float new_max);

private:

    std::string cabecera = "";

    std::vector<rgb> data;
    float max = 1.0;

    std::string name = "";
    int width = 0, height = 0, c = 0;

};

// Lee la imagen nombre
Image leer(std::string nombre);

// Escribe en formato ppm la imagen img, con nombre nombre, y
// con una resolucion col_res
void escribir(std::string nombre, Image& img, const int col_res);

// Escribe en formato ppm la imagen bmp, con nombre nombre, y
// con una resolucion col_res
void escribirbmp(std::string nombre, Image& img, const int col_res);

#endif