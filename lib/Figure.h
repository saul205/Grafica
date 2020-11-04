#ifndef FIGURE_H
#define FIGURE_H

#include "Ray.h"
#include "Rgb.h"
#include "DotDir.h"
#include "BoundingBox.h"

class Figure {
    private:

        rgb emission;

    public:

        virtual bool instersects(Ray ray, float& t, DotDir& p) = 0;
        virtual DotDir getCenter() = 0;
        virtual BoundingBox getBound() = 0;

        void setRgb(rgb _color){
            emission = _color;
        }

        rgb getEmission(){
            return emission;
        }
       
};

#endif