#include "Lab.hpp"

lab::lab(float _l, float _a, float _b){
    l = _l;
    a = _a;
    b = _b;
}

// Convierte la imagen de RGB a lab
void RGBToLab(Image& img, std::vector<lab>& data){

    data.clear();
    data.resize(img.getWidth()*img.getHeight());

    for(int i = 0; i < img.getHeight()*img.getWidth(); ++i){
        
        // Múltiples factores de conversión para obtener el LAB
        // a partir de los valores RGB
        rgb color = img.get(i);
        float r = color.r / 255.0f;
        float g = color.g / 255.0f;
        float b = color.b / 255.0f;

        r = (r > 0.04045f) ? pow(((r + 0.055f) / 1.055f), 2.4f) : r/12.92f;
        g = (g > 0.04045f) ? pow(((g + 0.055f) / 1.055f), 2.4f) : g/12.92f;
        b = (b > 0.04045f) ? pow(((b + 0.055f) / 1.055f), 2.4f) : b/12.92f;
        r *= 100.0f;
        g *= 100.0f;
        b *= 100.0f;

        float x = r*0.4124f + g*0.3576f + b*0.1805f;
        float y = r*0.2126f + g*0.7152f + b*0.0722f;
        float z = r*0.0193f + g*0.1192f + b*0.9505f;

        float refX = 95.047f;
        float refY = 100.000f;
        float refZ = 108.883f;

        x /= refX;
        y /= refY;
        z /= refZ;

        x = (x > 0.008856f) ? pow(x, 1.0f/3.0f) : (7.787f*x) + (16.0f/116.0f);
        y = (y > 0.008856f) ? pow(y, 1.0f/3.0f) : (7.787f*y) + (16.0f/116.0f);
        z = (z > 0.008856f) ? pow(z, 1.0f/3.0f) : (7.787f*z) + (16.0f/116.0f);

        data[i] = lab((116.0f*y) - 16.0f, 500.0f*(x-y), 200.0f*(y-z));
    }
}

// Convierte la imagen de lab a RGB
void LabToRGB(Image& img, std::vector<lab>& data){

    for(int i = 0; i < img.getHeight()*img.getWidth(); ++i){
        
        // Múltiples factores de conversión para obtener el RGB
        // a partir de los valores lab
        float refX = 95.047f;
        float refY = 100.000f;
        float refZ = 108.883f;
        float l = data[i].l;
        float a = data[i].a;
        float b = data[i].b;

        float y = (l + 16.0f) / 116.0f;
        float x = a / 500.0f + y;
        float z = y - b / 200.0f;

        y = (pow(y,3.0f) > 0.008856f) ? pow(y,3.0f) : (y - 16.0f / 116.0f) / 7.787f;
        x = (pow(x,3.0f) > 0.008856f) ? pow(x,3.0f) : (x - 16.0f / 116.0f) / 7.787f;
        z = (pow(z,3.0f) > 0.008856f) ? pow(z,3.0f) : (z - 16.0f / 116.0f) / 7.787f;

        x *= refX;
        y *= refY;
        z *= refZ;

        x /= 100.0f;
        y /= 100.0f;
        z /= 100.0f;

        float red = x*3.2406f + y*-1.5372f + z*-0.4986f;
        float green = x*-0.9689f + y*1.8758f + z*0.0415f;
        float blue = x*0.0557f + y*-0.2040f + z*1.0570f;

        red = (red > 0.0031308f) ? 1.055f*(pow(red, 1.0f / 2.4f )) - 0.055f : 12.92f*red;
        green = (green > 0.0031308f) ? 1.055f*(pow(green, 1.0f / 2.4f )) - 0.055f : 12.92f*green;
        blue = (blue > 0.0031308f) ? 1.055f*(pow(blue, 1.0f / 2.4f )) - 0.055f : 12.92f*blue;

        img.setRGB(i, rgb(red*255.0f, green*255.0f, blue*255.0f));
    }
}