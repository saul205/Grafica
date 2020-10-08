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

    void setData(std::string _cabecera, std::string _name, int _width, int _height, int _c, float _max){
        name = _name;
        width = _width;
        height = _height;
        c = _c;
        max = _max;
        cabecera = _cabecera;

        data.resize(width * height * 2);
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

    float getMax(){
        return max;
    }

    int getC(){
        return c;
    }

    void setMax(float _max){
        max = _max;
    }

    std::string toStringRow(int row){
        std::string ret = "";
        int index = width*row;
        for (int i = 0; i < width; ++i){
            ret += std::to_string((int)(data[index + i].r*(255 / max))) + " ";
            ret += std::to_string((int)(data[index + i].g*(255 / max))) + " ";
            ret += std::to_string((int)(data[index + i].b*(255 / max)));
            ret = (i+1 != width) ? ret + "     " : ret;
        }
        return ret;
    }

private:

    std::string cabecera = "";

    std::vector<rgb> data;
    float max = 0;

    std::string name = "";
    int width = 0, height = 0, c = 0;

};

#endif