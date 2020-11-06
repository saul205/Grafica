#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <string>
#include <vector>
#include <iostream>
#include "DotDir.h"
#include "Rgb.h"
#include "Figure.h"

class Triangle : public Figure {
    private:

        DotDir v0, v1, v2;
        DotDir v0v1, v0v2;

    public:

        Triangle() {}

        Triangle(const DotDir& _v0, const DotDir& _v1, const DotDir& _v2){
            v0 = _v0;
            v1 = _v1;
            v2 = _v2;
            v0v1 = v1 - v0;
            v0v2 = v2 - v0;
        }

        // Möller–Trumbore intersection algorithm, decide si el rayo intersecta o no con el triángulo
        bool intersects(Ray ray, float& t, DotDir& p) override {
    
            const float epsilon = 10e-7;
            DotDir h = crossProduct(ray.getDir(), v0v2);
            float det = dotProduct(v0v1, h);
    
            if (det > -epsilon && det < epsilon) return false;    // Rayo paralelo al triángulo
    
            float invDet = 1.0/det;
            DotDir s = ray.getOrigen() - v0;
            float u = invDet * dotProduct(s,h);
            
            if (u < 0.0 || u > 1.0) return false;
            
            DotDir q = crossProduct(s, v0v1);
            float v = invDet * dotProduct(ray.getDir(), q);
    
            if (v < 0.0 || u + v > 1.0) return false;
    
            // Sabemos que intersecta el rayo al triángulo en este punto
            // porque las coordenadas baricentricas u y v cumplen las restricciones
            t = invDet * dotProduct(v0v2, q);
    
            if (t > epsilon){ 
                p = ray.getOrigen() + t * ray.getDir();
                return true;
            } else { 
                return false;
            }
        }

        DotDir getCenter() override {
            return DotDir(
                (v1.getX() + v2.getX() + v0.getX()) / 3,
                (v1.getY() + v2.getY() + v0.getY()) / 3,
                (v1.getZ() + v2.getZ() + v0.getZ()) / 3,
                1
            );
        }

        BoundingBox getBound() override {

            float maxX = max( max(v0.getX(), v1.getX()), v2.getX());
            float maxY = max( max(v0.getY(), v1.getY()), v2.getY());
            float maxZ = max( max(v0.getZ(), v1.getZ()), v2.getZ());

            float minX = min( min(v0.getX(), v1.getX()), v2.getX());
            float minY = min( min(v0.getY(), v1.getY()), v2.getY());
            float minZ = min( min(v0.getZ(), v1.getZ()), v2.getZ());

            return BoundingBox( 
                DotDir(maxX, maxY, maxZ, 1),
                DotDir(minX, minY, minZ, 1)
                );
        }

};

#endif