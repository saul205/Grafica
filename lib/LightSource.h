#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "DotDir.h"
#include "Rgb.h"

class LightSource{
    private:

        float intensity;
        DotDir position;
        rgb color;

    public:

        LightSource(float _i, DotDir _position){
            intensity = _i;
            position = _position;
        }

        float getIntensity(){
            return intensity;
        }

        DotDir getPosition(){
            return position;
        }

        rgb getColor(){
            return color;
        }
};

#endif