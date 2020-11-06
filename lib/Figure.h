#ifndef FIGURE_H
#define FIGURE_H

#include "Ray.h"
#include "Rgb.h"
#include "Image.h"

class Figure{
    private:

        rgb emission;
        bool hasTexture = false;
    
    protected:

        Image textura;
        float new_max  = 1;
        int col_res = 255;

    public:

        Figure(){}

        virtual bool instersects(Ray ray, float& t, DotDir& p) = 0;

        virtual rgb getTexture(DotDir& interseccion) = 0;

        void setRgb(rgb _color){
            emission = _color;
        }

        rgb getEmission(DotDir& interseccion){
            if(!hasTexture){
                return (emission / col_res) * new_max;
            } else {
                return getTexture(interseccion) / new_max;
            }
        };

        void setTexture(const Image& _textura){
            textura = _textura;
            new_max = textura.getMaximo();
            hasTexture = true;
        }
};

#endif