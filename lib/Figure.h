#ifndef FIGURE_H
#define FIGURE_H

#include "Ray.h"
#include "Rgb.h"

class Figure{
    private:

        rgb emission;

    public:

        virtual bool instersects(Ray ray, float& t) = 0;

        void setRgb(rgb _color){
            emission = _color;
        }

        rgb getEmission(){
            return emission;
        };
};

#endif