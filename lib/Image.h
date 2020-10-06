#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include <iostream>

class Image{

private:

    struct rgb{
        float r, g, b;
    };

    std::vector<rgb> data;
    float max = 0;

    std::string name = "";
    int width = 0, height = 0, c = 0;
    
public:

    Image(){
    }

    void setData(std::string name, int width, int height, int c, float max){
        this->name = name;
        this->width = width;
        this->height = height;
        this->c = c;
        this->max = max;

        data.resize(width * height);
    }

    void set(int index, int r, int g, int b){
        if(0 <= index && index <= width * height){
            data[index] = {r * max / c, g * max/ c, b * max/ c};
            std::cout << index << std::endl;
        }else{
            std::cout << "Error en la insercion en rango: " << index << std::endl;
        }
    }

    rgb get(int index){
        if(0 <= index && index <= width * height){
            return data[index];
        }else{
            std::cout << "Error en el get en rango: " << index << std::endl;
        }
    }

    int getWidth(){
        return width;
    }

    int getHeight(){
        return height;
    }

    std::string getName(){
        return name;
    }

    float getMax(){
        return max;
    }

    int getC(){
        return c;
    }

    std::string toString(){
        return "Nombre: " + name + "\nWidth: " + std::to_string(width) + " Height: " + std::to_string(height);
    }

};

#endif