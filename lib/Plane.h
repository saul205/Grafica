#ifndef PLANE_H
#define PLANE_H

#include <string>
#include <vector>
#include <iostream>
#include "DotDir.h"
#include "Rgb.h"

class Plane {
    private:

        DotDir normal;
        float c;
        rgb emission;

    public:

        Plane() {}

        Plane(DotDir _normal, float _c){
            c = _c;
            normal = _normal;
        }

        // Debe tener una normal
        // Si no instersecta o está detrás de la cámara devuelve falso y no modifica t,
        // en caso contrario devuelve cierto y t = - (c + o*n) / (d*n)
        bool instersects(Ray ray, float& t){
    
            float den = dotProduct(ray.getDir(), normal);
            
            if(!den){
                // Intersecta en el infinito
                return false;
            }

            float num = -(c + dotProduct(ray.getOrigen(), normal));
            
            if(num / den < 0){
                return false;
            }

            t = num / den;
            return true;
        }

        //--------------------GETTERS-------------------------

        DotDir getNormal(){
            return normal;
        }

        float getC(){
            return c;
        }

        rgb getRgb(){
            return emission;
        }

        //--------------------SETTERS-------------------------

        void setRgb(rgb _color){
            emission = _color;
        }

        void setNormal(DotDir _normal){
            normal = _normal;
        }

        void setC(float _c){
            c = _c;
        }
};

#endif