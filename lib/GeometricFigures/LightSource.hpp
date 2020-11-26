#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "DotDir.hpp"
#include "Rgb.hpp"

class LightSource{
    private:

        rgb emission;
        DotDir position;

    public:

        LightSource(rgb _i, DotDir _position);

        DotDir getPosition();

        rgb getEmission();
};

#endif