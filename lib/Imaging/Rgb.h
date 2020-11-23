#ifndef RGB_H
#define RGB_H

#include <cmath>
#include <algorithm>


using namespace std;

struct rgb{

    float r = 0;
    float g = 0;
    float b = 0;

    rgb(){}

    rgb(float _r, float _g, float _b);

    float maximo();

    void set(float _r, float _g, float _b);
};

// Divide la r, la g y la b de color por f
rgb operator/(const rgb& color, const float& f);

// Multiplica la r, la g y la b de color por f
rgb operator*(const rgb& color, const float& f);

// Multiplica la r, la g y la b de color por f, alternativo
rgb operator*(const float& f, const rgb& color);

// Multiplica la r, la g y la b del primer elemento rgb
// por la r, la g, y la b del segundo elemento rgb
rgb operator*(const rgb& color1, const rgb& color2);

// Devuelve el resultado de sumar las componentes r, g y b de color1 y color2
rgb operator+(const rgb& color1, const rgb& color2);

#endif