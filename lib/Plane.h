#ifndef PLANE_H
#define PLANE_H

#include <string>
#include <vector>
#include <iostream>
#include "DotDir.h"
#include "Rgb.h"
#include "Figure.h"
#include "BoundingBox.h"

class Plane : public Figure {
    private:

        DotDir v1, v2, normal, center;
        float height, width;

    public:

        Plane() {}

        Plane(DotDir _v1, DotDir _v2, DotDir _center, float _height, float _width){
            height = _height;
            width = _width;
            v1 = _v1;
            v2 = _v2;
            center = _center;
            normal = crossProduct(v1, v2);
        }

        // Debe tener una normal
        // Si no instersecta o está detrás de la cámara devuelve falso y no modifica t,
        // en caso contrario devuelve cierto y t = - (c + o*n) / (d*n)
        bool intersects(Ray ray, float& t, DotDir& p) override {
    
            float den = dotProduct(ray.getDir(), normal);

            if(!den){
                // Intersecta en el infinito
                return false;
            }

            float num = dotProduct(center - ray.getOrigen(), normal);
            
            if(num / den <= 0){
                return false;
            }

            t = num / den;

            p = ray.getOrigen() + t * ray.getDir();

            DotDir v = p - center;
            
            float p1 = dotProduct(v, v2);
            float p2 = dotProduct(v, v1);

            if(p1 > - width / 2 && p1 < width / 2 && p2 > - height / 2 && p2 < height / 2){
                return true;
            }

            return false;
        }

        BoundingBox getBound() override {
            DotDir v2n = normalization(v2);
            DotDir v1n = normalization(v1);
            DotDir x = center + (width / 2 * v2n) + (height / 2 * v1n);
            DotDir y = center - (height / 2 * v1n) + (width / 2 * v2n);
            DotDir x2 = center - (width / 2 * v2n) - (height / 2 * v1n);
            DotDir y2 = center - (width / 2 * v2n) + (height / 2 * v1n);

            float maxX = max( max(x.getX(), y.getX()), max(x2.getX(), y2.getX()));
            float maxY = max( max(x.getY(), y.getY()), max(x2.getY(), y2.getY()));
            float maxZ = max( max(x.getZ(), y.getZ()), max(x2.getZ(), y2.getZ()));

            float minX = min( min(x.getX(), y.getX()), min(x2.getX(), y2.getX()));
            float minY = min( min(x.getY(), y.getY()), min(x2.getY(), y2.getY()));
            float minZ = min( min(x.getZ(), y.getZ()), min(x2.getZ(), y2.getZ()));

            return BoundingBox(
                DotDir(maxX, maxY, maxZ, 1),
                DotDir(minX, minY, minZ, 1)
                );
        }

        //--------------------GETTERS-------------------------

        DotDir getNormal(){
            return normal;
        }

        DotDir getCenter() override {
            return center;
        }

        //--------------------SETTERS-------------------------

        void setNormal(DotDir _normal){
            normal = _normal;
        }
};

#endif