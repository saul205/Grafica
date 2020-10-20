#ifndef RAY_H
#define RAY_H

#include "DotDir.h"

class Ray{
    private:

        DotDir origen;
        DotDir direction;

    public:

        Ray(DotDir _origen, DotDir direccion){
            origen = _origen;
            direction = direccion;
        }

        DotDir getDir(){
            return direction;
        }

        DotDir getOrigen(){
            return origen;
        }
};

#endif