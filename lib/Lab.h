#ifndef LAB_H
#define LAB_H

#include "Rgb.h"
#include "Image.h"

struct lab{
    float l = 0.0f;
    float a = 0.0f;
    float b = 0.0f;

    lab(float _l, float _a, float _b){
        l = _l;
        a = _a;
        b = _b;
    }

    lab(){}
};


void RGBToLab(Image& img, std::vector<lab>& data){

    data.clear();
    data.resize(img.getWidth()*img.getHeight());

    for(int i = 0; i < img.getHeight()*img.getWidth(); ++i){
        
        rgb color = img.get(i);
        float r = color.r / 255.0f;
        float g = color.g / 255.0f;
        float b = color.b / 255.0f;

        r = (r > 0.04045f) ? pow(((r + 0.055f) / 1.055f), 2.4) : r/12.92f;
        g = (g > 0.04045f) ? pow(((g + 0.055f) / 1.055f), 2.4) : g/12.92f;
        b = (b > 0.04045f) ? pow(((b + 0.055f) / 1.055f), 2.4) : b/12.92f;
        r *= 100;
        g *= 100;
        b *= 100;

        float x = r*0.4124 + g*0.3576 + b*0.1805;
        float y = r*0.2126 + g*0.7152 + b*0.0722;
        float z = r*0.0193 + g*0.1192 + b*0.9505;

        float refX = 94.811f;
        float refY = 100.000f;
        float refZ = 107.304f;

        x /= refX;
        y /= refY;
        z /= refZ;

        x = (x > 0.008856f) ? pow(x, 1.0f/3.0f) : (7.787f*x) + (16.0f/116.0f);
        y = (y > 0.008856f) ? pow(y, 1.0f/3.0f) : (7.787f*y) + (16.0f/116.0f);
        z = (z > 0.008856f) ? pow(z, 1.0f/3.0f) : (7.787f*z) + (16.0f/116.0f);

        data[i] = lab((116.0f*y) - 16.0f, 500.0f*(x-y), 200.0f*(y-z));
    }
}

void LabToRGB(Image& img, std::vector<lab>& data){

    for(int i = 0; i < img.getHeight()*img.getWidth(); ++i){
        
        float refX = 94.811f;
        float refY = 100.000f;
        float refZ = 107.304f;
        float l = data[i].l;
        float a = data[i].a;
        float b = data[i].b;

        float y = (l + 16.0f) / 116.0f;
        float x = a / 500.0f + y;
        float z = y - b / 200.0f;

        y = (pow(y,3) > 0.008856) ? pow(y,3) : (y - 16.0f / 116.0f) / 7.787f;
        x = (pow(x,3) > 0.008856) ? pow(x,3) : (x - 16.0f / 116.0f) / 7.787f;
        z = (pow(z,3) > 0.008856) ? pow(z,3) : (z - 16.0f / 116.0f) / 7.787f;

        x *= refX;
        y *= refY;
        z *= refZ;

        x /= 100.0f;
        y /= 100.0f;
        z /= 100.0f;

        float red = x*3.2406 + y*-1.5372 + z*-0.4986;
        float green = x*-0.9689 + y*1.8758 + z*0.0415;
        float blue = x*0.0557 + y*-0.2040 + z*1.0570;

        red = (red > 0.0031308) ? 1.055*(pow(red, 1.0f / 2.4f )) - 0.055f : 12.92f*red;
        green = (green > 0.0031308) ? 1.055*(pow(green, 1.0f / 2.4f )) - 0.055f : 12.92f*green;
        blue = (blue > 0.0031308) ? 1.055*(pow(blue, 1.0f / 2.4f )) - 0.055f : 12.92f*blue;

        img.setRGB(i, rgb(red*255.0f, green*255.0f, blue*255.0f));
    }
}

#endif