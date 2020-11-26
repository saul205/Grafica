#ifndef DOTDIR_H
#define DOTDIR_H

#include <math.h>
#include <string>
#include <stdlib.h> /* abs */
#include <iostream>

using namespace std;

class DotDir
{
private:
  float c[4];

public:
  friend DotDir operator-(const DotDir &dd1, const DotDir &dd2);
  friend DotDir operator+(const DotDir &dd1, const DotDir &dd2);
  friend DotDir operator*(float t, const DotDir &dd1);
  friend DotDir operator*(const DotDir &dd1, float t);
  friend DotDir operator/(const DotDir &dd1, float div);
  friend float dotProduct(const DotDir &dd1, const DotDir &dd2);
  friend DotDir crossProduct(const DotDir &dd1, const DotDir &dd2);
  friend DotDir normalization(const DotDir &dd);

  DotDir() {}

  DotDir(float x, float y, float z, float w);

  // Getters de los componentes
  float getX() const;
  float getY() const;
  float getZ() const;
  float getW() const;

  void setDotDir(float x, float y, float z, float w);

  // Devuelve el módulo
  float mod() const;

  string toString() const;

  // i >= 0 AND i <= 3
  float operator[](const int i) const;

  // i >= 0 AND i <= 3
  float operator[](int i);
};

DotDir operator/(const DotDir &dd1, float div);

// Dirección + Dirección = Dirección OK, w = 0
// Punto + Punto = NO HACER NO OK, w = 0
// Dirección + Punto = Punto OK, w = 1
// Punto + Dirección = Punto OK, w = 1
DotDir operator+(const DotDir &dd1, const DotDir &dd2);

// Dirección - Dirección = Dirección OK, w = 0
// Punto - Punto = Dirección OK, w = 0
// Dirección - Punto = NO HACER NO OK, w = - 1
// Punto - Dirección = Punto OK, w = 1
DotDir operator-(const DotDir &dd1, const DotDir &dd2);

DotDir operator*(const DotDir &dd1, float t);

DotDir operator*(float t, const DotDir &dd1);

// Devuelve el producto escalar de los vectores d1 y d2.
float dotProduct(const DotDir &dd1, const DotDir &dd2);

// Devuelve el producto vectorial de los vectores d1 y d2. Obviamente, d1 y d2
// deben ser direcciones
DotDir crossProduct(const DotDir &dd1, const DotDir &dd2);

// Devuelve el vector correspondiente a la normalización de la dirección d;

// Calcula el ángulo entre dos vectores
float angle(const DotDir &dd1, const DotDir &dd2);

#endif
