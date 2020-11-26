#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include <fstream>
#include "DotDir.hpp"
#include <vector>
#include "Triangle.hpp"
#include <iostream>
#include "Transformation.hpp"

class TriangleMesh{

    private:

        std::vector<Triangle> triangulos;
        DotDir center, top, bottom;

        // Aplica la transformación t a todos los triángulos
        void transform(Transformation t);

    public:

        TriangleMesh(){}

        // Lee una malla de triángulos compuestas únicamente de triángulos
        // no coloreados del fichero file.
        void read(string file);

        // Devuelve el triángulo de índice n de la lista
        Triangle operator[](int n);

        // Devuelve el númerod e triángulos de la malla
        float getSize() const;

        // Desplaza la malla al nuevo punto
        void move(DotDir newCenter);

        // Escala la malla de triángulos
        void scale(float x, float y, float z);
};

#endif