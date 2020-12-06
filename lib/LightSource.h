#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "DotDir.h"
#include "Rgb.h"

class LightSource{
    private:

        DotDir position;
        rgb emission;

    public:

        LightSource(rgb _i, DotDir _position){
            emission = _i;
            position = _position;
        }

        DotDir getPosition(){
            return position;
        }

        rgb getEmission(){
            return emission;
        }
};

#endif