#ifndef RAY_H
#define RAY_H

#include "DotDir.hpp"

class Ray{
    private:

        DotDir origen;
        DotDir direction;

    public:

        Ray(DotDir _origen, DotDir direccion);

        DotDir getDir();
        
        DotDir getOrigen();
};

#endif