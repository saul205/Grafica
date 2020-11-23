#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "DotDir.h"

class Transformation{
  private:
    float matriz[4][4] = {1,0,0,0,
                          0,1,0,0,
                          0,0,1,0,
                          0,0,0,1};
  public:

    Transformation() {}

    Transformation(float mat[4][4]);

    friend Transformation operator*(const Transformation& t1, const Transformation& t2);
    friend DotDir operator*(const Transformation& t, const DotDir& dd);
    friend Transformation transpuesta(const Transformation& t1);
    friend Transformation inverse(const Transformation& t1);
    
    // Rellena la diagonal de la matriz con ceros
    void setZero();

    // Set de la matriz a la matriz identidad
    void identidad();

    // Set de la  matriz de traslacion
    void translation(float x, float y, float z);

    // Set de la  matriz de escala
    void scale(float x, float y, float z);

    // Set de la  matriz de rotacion eje X
    void rotationX(float theta);

    // Set de la  matriz de rotacion eje Y
    void rotationY(float theta);

    // Set de la  matriz de rotacion eje Z
    void rotationZ(float theta);

    // Set de la  matriz de cambio de base formada por los vectores u,v,w y centrada
    // en el punto o.
    void changeBase(const DotDir& u, const DotDir& v, const DotDir& w, const DotDir& o);

    string toString() const;
};


// Devuelve la matriz transpuesta
Transformation transpuesta(const Transformation& t1);

// Devuelve el producto de transformaciones (matrices)
Transformation operator*(const Transformation& t1, const Transformation& t2);

// Devuelve producto de transformacion (matriz) por vector
DotDir operator*(const Transformation& t, const DotDir& dd);

// Calculo del determinante de 3x3
float Sarrus(float mat[3][3]);

// Obtenemos los cofactores para el calculo del determinante 4x4
void cofactor(float mat[4][4], float cof[3][3], int q);

// Calculo del determinante 4x4
float determinante(float mat[4][4]);

//Devuelve la inversa de una transformación, si existe.
Transformation inverse(const Transformation &t1);

#endif