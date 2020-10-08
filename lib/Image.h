#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include <iostream>

class Image{
    
public:

    struct rgb{
        float r, g, b;
    };

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
        data[index] = {r * max / c, g * max/ c, b * max/ c};
    }

    rgb get(int index){
        return data[index];
    }

    int getWidth(){
        return width;
    }

    std::string getCabecera(){
        return cabecera;
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
            Image::rgb colores = get(i);
            if(colores.r > maximo){ maximo = colores.r; }
            if(colores.g > maximo){ maximo = colores.g; }
            if(colores.b > maximo){ maximo = colores.b; }
        }
        return maximo;
    }

    /*std::string toString(const int col_res, const float new_max){
        std::string ret = "";
        for(int i = 0; i < height; ++i){
            for (int j = 0; j < width; ++j){
                ret += std::to_string((int)(data[width*i + j].r*(col_res / new_max))) + " ";
                ret += std::to_string((int)(data[width*i + j].g*(col_res / new_max))) + " ";
                ret += std::to_string((int)(data[width*i + j].b*(col_res / new_max)));
                ret = (j+1 != width) ? ret + "     " : ret + "\n";
            }
        }
        return ret;
    }*/

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

        write += img.getCabecera() + "\n";
        write += "#MAX=" + std::to_string(maximo) + "\n";
        write += std::to_string(img.getWidth()) + " " + std::to_string(img.getHeight()) + "\n";
        write += std::to_string(col_res) + "\n";
        f << write;
        
        for(int i = 0; i < img.getHeight(); ++i){
            f << img.toStringRow(i, col_res, maximo) << "\n";
        }
    }
}
#endif