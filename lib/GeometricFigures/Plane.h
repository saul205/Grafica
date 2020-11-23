#ifndef PLANE_H
#define PLANE_H

#include <string>
#include <vector>
#include <iostream>
#include "Figure.h"

class Plane : public Figure {
    private:

        DotDir v1, v2, normal, center;
        DotDir v1n, v2n;
        float height, width;

    public:

        Plane() {}

        Plane(DotDir _v1, DotDir _v2, DotDir _center, float _height, float _width);

        // Debe tener una normal
        // Si no instersecta o está detrás de la cámara devuelve falso y no modifica t,
        // en caso contrario devuelve cierto y t = - (c + o*n) / (d*n)
        bool intersects(Ray ray, float& t, DotDir& p) override;

        // Obtiene la boundingBox del plano finito
        BoundingBox getBound() override;

        //--------------------GETTERS-------------------------

        DotDir getNormal();

        DotDir getCenter() override;

        // Obtiene la base de la intersección con el plano
        void getBase(DotDir interseccion, DotDir& base0, DotDir& base1, DotDir& base2) override;

        //--------------------SETTERS-------------------------

        void setNormal(DotDir _normal);
  
        // Obtiene el valor correspondiente en la textura
        rgb getTexture(const DotDir& interseccion) override;

};

#endif