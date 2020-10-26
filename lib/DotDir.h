#ifndef DOTDIR_H
#define DOTDIR_H

#include <math.h>
#include <string>
#include <stdlib.h>     /* abs */
#include <iostream>

using namespace std;

class DotDir {
  private:
    float c[4];
  public:

     friend DotDir operator-(const DotDir& dd1, const DotDir& dd2);
     friend DotDir operator+(const DotDir& dd1, const DotDir& dd2);
     friend DotDir operator*(float t, const DotDir& dd1);
     friend float dotProduct(const DotDir& dd1, const DotDir& dd2);
     friend DotDir crossProduct(const DotDir& dd1, const DotDir& dd2);
     friend DotDir normalization(const DotDir& dd);

     DotDir(){}

     DotDir(float x, float y, float z, float w){
         c[0] = x;
         c[1] = y;
         c[2] = z;
         c[3] = w;
     }

     float getX() const {
       return c[0];
     }

     float getY() const {
       return c[1];
     }

     float getZ() const {
       return c[2];
     }

     float getW() const {
       return c[3];
     }

     void setDotDir(float x, float y, float z, float w){
         c[0] = x;
         c[1] = y;
         c[2] = z;
         c[3] = w;
     }

     float mod() const {
         return sqrt(c[0] * c[0] + c[1] * c[1] + c[2] * c[2]);
     }

     string toString() const{
       return to_string(c[0]) + ", " + to_string(c[1]) + ", " + to_string(c[2]) + ", " + to_string(c[3]);
     }
};

DotDir operator+(const DotDir& dd1, const DotDir& dd2){
  // Dirección + Dirección = Dirección OK, w = 0
  // Punto + Punto = NO HACER NO OK, w = 0
  // Dirección + Punto = Punto OK, w = 1
  // Punto + Dirección = Punto OK, w = 1
  return DotDir(dd1.c[0] + dd2.c[0], dd1.c[1] + dd2.c[1], dd1.c[2] + dd2.c[2], dd1.c[3] + dd2.c[3]);
}

DotDir operator-(const DotDir& dd1, const DotDir& dd2){
  // Dirección - Dirección = Dirección OK, w = 0
  // Punto - Punto = Dirección OK, w = 0
  // Dirección - Punto = NO HACER NO OK, w = - 1
  // Punto - Dirección = Punto OK, w = 1
  return DotDir(dd1.c[0] - dd2.c[0], dd1.c[1] - dd2.c[1], dd1.c[2] - dd2.c[2], dd1.c[3] - dd2.c[3]);
}

DotDir operator*(float t, const DotDir& dd1){
  // Dirección - Dirección = Dirección OK, w = 0
  // Punto - Punto = Dirección OK, w = 0
  // Dirección - Punto = NO HACER NO OK, w = - 1
  // Punto - Dirección = Punto OK, w = 1
  return DotDir(dd1.c[0] * t, dd1.c[1] * t, dd1.c[2] * t, dd1.c[3]);
}

// Devuelve el producto escalar de los vectores d1 y d2.
float dotProduct(const DotDir& dd1, const DotDir& dd2){
  return dd1.c[0]*dd2.c[0] + dd1.c[1]*dd2.c[1] + dd1.c[2]*dd2.c[2];
}

// Devuelve el producto vectorial de los vectores d1 y d2. Obviamente, d1 y d2
// deben ser direcciones
DotDir crossProduct(const DotDir& dd1, const DotDir& dd2){
  return DotDir(dd1.c[1]*dd2.c[2] - dd1.c[2]*dd2.c[1],
                dd1.c[2]*dd2.c[0] - dd1.c[0]*dd2.c[2],
                dd1.c[0]*dd2.c[1] - dd1.c[1]*dd2.c[0], 0);
}

// Devuelve el vector correspondiente a la normalización de la dirección d
DotDir normalization(const DotDir& dd){
  float modulo = dd.mod();
  return DotDir(dd.c[0]/modulo, dd.c[1]/modulo, dd.c[2]/modulo, dd.c[3]);
}

// Calcula el ángulo entre dos vectores
float angle(const DotDir& dd1, const DotDir& dd2){
      float angulo = dotProduct(dd1, dd2);
      angulo = angulo / dd1.mod();
      angulo = angulo / dd2.mod();
      angulo = acos(angulo);
      return angulo;
}

#endif
