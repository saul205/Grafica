#ifndef RGB_H
#define RGB_H

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

#endif