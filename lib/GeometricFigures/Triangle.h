#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <string>
#include <vector>
#include <iostream>
#include "Figure.h"

struct triangleVertexUV{

    // Los marcados con uno corresponden al vértice A
    // Los marcados con dos corresponden al vértice B
    // Los marcados con tres corresponden al vértice C
    // Para hacer el siguiente triángulo:
    /*
            A -------------- B
            -              -
            -            -          
            -         -             Dejar coordenadas por default
            -      -                    A = (0,0)
            -    -                      B = (1,0)
            -  -                        C = (1,1)
            C

        Para hacer un plano hacer un triángulo por defecto y otro:

                                B
                            -   -       Con coordenadas:
                        -       -           A = (1,1)
                    -           -           B = (1,0)
                -               -           C = (0,1)
            C--------------------A
    */

    float u1 = 0;
    float u2 = 1; 
    float u3 = 0;
    float v1 = 0; 
    float v2 = 0;
    float v3 = 1; 

    triangleVertexUV(){}

    triangleVertexUV(float _u1, float _v1, float _u2, float _v2, float _u3, float _v3);
};

class Triangle : public Figure {
    private:

        DotDir v0, v1, v2;
        DotDir v0v1, v0v2;
        DotDir normal, center;
        triangleVertexUV tvUV;

    public:

        Triangle() {}

        Triangle(const DotDir& _v0, const DotDir& _v1, const DotDir& _v2);

        Triangle(const DotDir& _v0, const DotDir& _v1, const DotDir& _v2, const triangleVertexUV& _tvUV);

        // Möller–Trumbore intersection algorithm, chequea si el rayo intersecta o no con el triángulo
        bool intersects(Ray ray, float& t, DotDir& p) override;
        
        // INtersection DEBE intersectar al triángulo
        rgb getTexture(const DotDir& p) override;

        // Devuelve el centro del triángulo
        DotDir getCenter() override ;

        // Devuelve una base creada en el punto interseccion
        void getBase(DotDir interseccion, DotDir& base0, DotDir& base1, DotDir& base2) override;

        // Devuelve la bounding box del triángulo
        BoundingBox getBound() override;

        // Aplica la transformacion t al triángulo
        void transform(Transformation t) override;

};

#endif