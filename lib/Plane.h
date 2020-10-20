#ifndef PLANE_H
#define PLANE_H

#include <string>
#include <vector>
#include <iostream>
#include "DotDir.h"
#include "Rgb.h"

class Plane{
    private:

        DotDir normal;
        float distance;
        rgb emission;

    public:

        Plane(DotDir _normal, float _distance){
            distance = _distance;
            normal = _normal;
        }

        //--------------------GETTERS-------------------------

        DotDir getNormal(){
            return normal;
        }

        float getDistance(){
            return distance;
        }

        rgb getRgb(){
            return emission;
        }

        //--------------------SETTERS-------------------------

        void setRgb(rgb _color){
            emission = _color;
        }
};

#endif