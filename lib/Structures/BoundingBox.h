#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "DotDir.h"
#include "Ray.h"
#include <float.h>

class BoundingBox{

    private:

        DotDir top, bottom;

    public:

        friend BoundingBox Union(const BoundingBox& box1, const BoundingBox& box2);
        friend BoundingBox Union(const BoundingBox& box1, const DotDir& box2);

        BoundingBox();

        // Crea una bounding box definida por los componentes máximos y mínimos
        // de cada uno de los puntos para las tres coordenadas espaciales
        BoundingBox(DotDir _top, DotDir _bottom);

        // Devuelve cierto si y solo si el rayo intersecta a la bounding box.
        // Además, si esto ocurre, t vale la distancia del origen del rayo al 
        // punto de intersección
        bool intersects(Ray ray, float& t);
        bool intersects(Ray ray, float& t) const;

        // Devuelve el centro de la bounding box
        DotDir getCenter();

        DotDir getTop();
        DotDir getBottom();

        // Devuelve el eje a partir, el que es mayor de los tres
        // Devuelve 0, 1 o 2
        int MaxAxe();

};

// Devuelve una bounding box que contiene box1 y box2
BoundingBox Union(const BoundingBox& box1, const BoundingBox& box2);

// Devuelve una bounding box que contiene box1 y box2
BoundingBox Union(const BoundingBox& box1, const DotDir& box2);


#endif