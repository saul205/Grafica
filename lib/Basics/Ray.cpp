#include "Ray.hpp"

Ray::Ray(DotDir _origen, DotDir direccion)
{
    origen = _origen;
    direction = direccion;
}

DotDir Ray::getDir()
{
    return direction;
}

DotDir Ray::getOrigen()
{
    return origen;
}
