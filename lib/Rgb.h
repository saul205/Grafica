#ifndef RGB_H
#define RGB_H

#include <algorithm>

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

struct hsl{
    float h = 0;
    float s = 0;
    float l = 0;

    hsl(float _h, float _s, float _l){
        h = _h;
        s = _s;
        l = _l;
    }

    hsl(){}
};

hsl rgbToHsl(rgb color){
    hsl transformed;

    float red = color.r / 255;
    float green = color.g / 255;
    float blue = color.b / 255;

    float max_rgb = std::max({red, green, blue});
    float min_rgb = std::min({red, green, blue});
    float del_rgb = max_rgb - min_rgb;

    transformed.l = ( max_rgb + min_rgb ) / 2;

    if( del_rgb != 0){
        if( transformed.l < 0.5 ){
            transformed.s = del_rgb / ( max_rgb + min_rgb );
        }else{
            transformed.s = del_rgb / ( 2 - max_rgb - min_rgb );
        }

        if( red == max_rgb ){
            transformed.h = 60 * ((int)((green - blue) / del_rgb) % 6);
        }
        else if ( green == max_rgb ){
            transformed.h = 60 * (((blue - red) / del_rgb) + 2);
        }
        else if ( blue == max_rgb ){
            transformed.h = 60 * (((red - green) / del_rgb) + 4);
        }
    }
    
    return transformed;
}

float hueToRgb(float v1, float v2, float vH){
    if ( vH < 0 ) vH += 1;
    if ( vH > 1 ) vH -= 1;

    if ( ( 6 * vH ) < 1 ){
        return ( v1 + ( v2 - v1 ) * 6 * vH );
    }

    if ( ( 2 * vH ) < 1 ){
        return ( v2 );
    }

    if ( ( 3 * vH ) < 2 ){
        return ( v1 + ( v2 - v1 ) * ( ( 2.0f / 3 ) - vH ) * 6 );
    }

    return ( v1 );
}

rgb hslToRgb(hsl color){

    rgb colorRGB;

    if(color.s != 0){

        color.h = color.h / 360;

        float var1, var2;

        if( color.l < 0.5 ){
            var2 = color.l * ( 1 + color.s );
        }else{
            var2 = ( color.l + color.s ) - ( color.s * color.l );
        }

        var1 = 2 * color.l - var2;

        //cout << color.h << " " << var1 << " " << var2 << endl;

        colorRGB.r = 255 * hueToRgb(var1, var2, color.h + (1.0f / 3));
        colorRGB.g = 255 * hueToRgb(var1, var2, color.h);
        colorRGB.b = 255 * hueToRgb(var1, var2, color.h - (1.0f / 3));
    }
    else {
        colorRGB.r = color.l * 255;
        colorRGB.g = color.l * 255;
        colorRGB.b = color.l * 255;
    }

    return colorRGB;
}

#endif