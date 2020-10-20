#ifndef RAY_H
#define RAY_H

#include "DotDir.h"

class Ray{
    private:

        DotDir point;
        DotDir direction;

    public:

        Ray(DotDir punto, DotDir direccion){
            point = punto;
            direction = direccion;
        }
};

#endif