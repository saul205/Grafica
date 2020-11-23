#ifndef LAB_H
#define LAB_H

#include "Image.h"

struct lab{

    float l = 0.0f;
    float a = 0.0f;
    float b = 0.0f;

    lab(float _l, float _a, float _b);

    lab(){}
};

// Convierte img de RGB a formato Lab
// Valores de referencia obtenidos de http://www.easyrgb.com/en/math.php#text8
void RGBToLab(Image& img, std::vector<lab>& data);

// Convierte img de formato RGB a Lab
// Valores de referencia obtenidos de http://www.easyrgb.com/en/math.php#text8
void LabToRGB(Image& img, std::vector<lab>& data);

#endif