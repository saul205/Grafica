#ifndef FIGUREPROPERTIES_H
#define FIGUREPROPERTIES_H

#include "DotDir.h"
#include "Rgb.h"
#include <cmath>

class FigureProperties{
    
    private:

        float refractionIndex = 2.0f;

    public:

        // Obtiene el ángulo de refracción respecto a la normal
        // si tita1 es el ángulo de incidencia respecto a la misma
        float getSnellRefractionAngle(float tita1){
            return asin(sin(tita1) / refractionIndex);
        }

};

// Obtiene el rayo especular 
DotDir getSpecularRay(DotDir normal, DotDir wo){
    return wo - 2.0f*(dotProduct(wo,normal))*normal;
}

#endif