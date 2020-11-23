#include "LightSource.h"

LightSource::LightSource(rgb _i, DotDir _position)
{
    emission = _i;
    position = _position;
}

DotDir LightSource::getPosition()
{
    return position;
}

rgb LightSource::getEmission()
{
    return emission;
}
