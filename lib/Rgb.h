#ifndef RGB_H
#define RGB_H

#include <cmath>


using namespace std;

struct rgb{

    float r = 0;
    float g = 0;
    float b = 0;

    rgb(float _r, float _g, float _b){ 
        r = _r;
        g = _g;
        b = _b;
    }

    float maximo(){
        return max(max(r, b),g);
    }

    rgb(){}

    void set(float _r, float _g, float _b){ 
        r = _r;
        g = _g;
        b = _b;
    }
};

rgb operator/(const rgb& color, const float& f){
    return rgb(color.r / f, color.g / f, color.b / f);
}

rgb operator*(const rgb& color, const float& f){
    return rgb(color.r * f, color.g * f, color.b * f);
}

rgb operator*(const float& f, const rgb& color){
    return rgb(color.r * f, color.g * f, color.b * f);
}

rgb operator*(const rgb& color1, const rgb& color2){
    return rgb(color1.r * color2.r, color1.g * color2.g, color1.b * color2.b);
}

rgb operator+(const rgb& color1, const rgb& color2){
    return rgb(color1.r + color2.r, color1.g + color2.g, color1.b + color2.b);
}

#endif