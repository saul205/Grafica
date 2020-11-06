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

    rgb(){}
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

#endif