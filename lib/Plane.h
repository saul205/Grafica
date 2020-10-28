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

        DotDir normal;
        float lim[6];
        float c;

    public:

        Plane() {}

        Plane(DotDir _normal, float _lim[], float _c){
            c = _c;

            for(int i = 0; i < 6; i++){
                lim[i] = _lim[i];
            }

            normal = _normal;
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

            float num = -(c + dotProduct(ray.getOrigen(), normal));
            
            if(num / den <= 0){
                return false;
            }

            t = num / den;

            p = ray.getOrigen() + t * ray.getDir();


            //cout << p.toString() << endl;

            if(p.getX() <= lim[1] && p.getX() >= lim[0] &&
                p.getY() <= lim[3] && p.getY() >= lim[2] &&
                p.getZ() <= lim[5] && p.getZ() >= lim[4]){
                    return true;
                }

            //cout << num << " / " << den << " = " << t << endl;
            //cout << ray.getDir().toString() << endl;
            return false;
        }

        //--------------------GETTERS-------------------------

        DotDir getNormal(){
            return normal;
        }

        float getC(){
            return c;
        }

        //--------------------SETTERS-------------------------

        void setNormal(DotDir _normal){
            normal = _normal;
        }

        void setC(float _c){
            c = _c;
        }
};

#endif