#ifndef PLANE_H
#define PLANE_H

#include <string>
#include <vector>
#include <iostream>
#include "DotDir.h"
#include "Rgb.h"
#include "Figure.h"

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
        bool instersects(Ray ray, float& t, DotDir& p) override {
    
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

        //--------------------GETTERS-------------------------

        DotDir getNormal(){
            return normal;
        }

        //--------------------SETTERS-------------------------

        void setNormal(DotDir _normal){
            normal = _normal;
        }
};

#endif