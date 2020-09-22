#ifndef esfera
#define esfera

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
     friend DotDir dotProduct(const DotDir d1, const DotDir d2);
     friend DotDir crossProduct(const DotDir d1, const DotDir d2);

     DotDir(){}

     DotDir(float x, float y, float z, float w){
         c[0] = x;
         c[1] = y;
         c[2] = z;
         c[3] = w;
     }

     float getX(){
       return c[0];
     }

     float getY(){
       return c[1];
     }

     float getZ(){
       return c[2];
     }

     float getW(){
       return c[3];
     }

     int mod() {
         return sqrt(c[0] * c[0] + c[1] * c[1] + c[2] * c[2]);
     }

     string ToString(){
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

// Devuelve el producto escalar de los vectores d1 y d2. Obviamente, d1 y d2
// deben ser vectores
DotDir dotProduct(const DotDir d1, const DotDir d2){
  return DotDir(d1.c[0]*d2.c[0], d1.c[1]*d2.c[1], d1.c[2]*d2.c[2], d1.c[3]*d2.c[3]);
}

// Devuelve el producto vectorial de los vectores d1 y d2. Obviamente, d1 y d2
// deben ser vectores
DotDir crossProduct(const DotDir d1, const DotDir d2){
<<<<<<< HEAD
  return DotDir(d1.c[1]*d2.c[2] - d1.c[2]*d2.c[1], 
                d1.c[2]*d2.c[0] - d1.c[0]*d2.c[2], 
                d1.c[0]*d2.c[1] - d1.c[1]*d2.c[0], d1.c[3] * d2.c[3]);
=======
  return DotDir(d1.c[1]*d2.c[2] - d1.c[2]-d2.c[1],
                d1.c[2]*d2.c[0] - d1.c[0]-d2.c[2],
                d1.c[0]*d2.c[1] - d1.c[1]-d2.c[0], d1.c[3] * d2.c[3]);
>>>>>>> 448514668ffb943b561159a90e70c0bb62afe1d4
}

class Sphere{
  private:
    DotDir sphereCenter, sphereAxis, sphereCity;
  public:

    Sphere(){}
    // Los parámetros center, axis y city deben cumplir
    // checkRadius(center, axis, city)
    Sphere(const DotDir& center, const DotDir& axis, const DotDir& city){
      sphereCenter = center;
      sphereAxis = axis;
      sphereCity = city;
    }

    void getBase(DotDir base[3]){
      base[0] = sphereAxis;
      base[1] = crossProduct(sphereAxis, sphereCity - sphereCenter);
      base[2] = crossProduct(base[0], base[1]);
    }
};


// Devuelve cierto si y solo si el radio definido por el eje del planeta,
// el cual corresponde a su diámetro (debe ser una dirección),
// y la distancia entre el centro de la esfera y la ciudad de referencia
// (ambos deben ser puntos) difieren en menos de 10e-6.
bool checkRadius(DotDir axis, DotDir center, DotDir city){
  DotDir radius = center - city;
  cout << radius.mod() << endl;
  cout << axis.mod() << endl;
  return (abs(radius.mod() - 0.5*(axis.mod())) < 0.000001) ? true : false;
}

class Transformation{
  private:
    float matriz[4][4] = {1,0,0,0,
                          0,1,0,0,
                          0,0,1,0,
                          0,0,0,1};
  public:

    Transformation() {}

    // Set de la matriz a la matriz identidad
    void identidad(){
      matriz = {1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                0,0,0,1};
    }

    // Set de la  matriz de traslacion
    void translation(float x, float y, float z){
      matriz[0][3] = x;
      matriz[1][3] = y;
      matriz[2][3] = z;
    }

    // Set de la  matriz de escala
    void scale(float x, float y, float z){
      matriz[0][0] = x;
      matriz[1][1] = y;
      matriz[2][2] = z;
    }

    // Set de la  matriz de rotacion eje X
    void rotationX(float theta){
      matriz[1][1] = cos(theta);
      matriz[1][2] = -sin(theta);
      matriz[2][2] = cos(theta);
      matriz[2][1] = sin(theta);
    }

    // Set de la  matriz de rotacion eje Y
    void rotationY(float theta){
      matriz[0][0] = cos(theta);
      matriz[2][0] = -sin(theta);
      matriz[2][2] = cos(theta);
      matriz[0][2] = sin(theta);
    }

    // Set de la  matriz de rotacion eje Z
    void rotationZ(float theta){
      matriz[0][0] = cos(theta);
      matriz[0][1] = -sin(theta);
      matriz[1][1] = cos(theta);
      matriz[1][0] = sin(theta);
    }

    // Set de la  matriz de cambio de base formada por los vectores u,v,w y centrada
    // en el punto o.
    void changeBase(const DotDir& u, const DotDir& v, const DotDir& w, const DotDir& o){
      matriz[0][0] = u.getX();
      matriz[1][0] = u.getY();
      matriz[2][0] = u.getZ();
      matriz[0][1] = v.getX();
      matriz[1][1] = v.getY();
      matriz[2][1] = v.getZ();
      matriz[0][2] = w.getX();
      matriz[1][2] = w.getY();
      matriz[2][2] = w.getZ();
      matriz[0][3] = o.getX();
      matriz[1][3] = o.getY();
      matriz[2][3] = o.getZ();
    }
};

Transformation operator*(const Transformation& t1, const Transformation& t2){
  Transformation producto;
  producto[0][0] = t1[0][0]*t2[0][0] + t1[0][1]*t2[1][0] + t1[0][2]*t2[2][0] + t1[0][3]*t2[3][0];
  producto[0][1] = t1[0][0]*t2[0][1] + t1[0][1]*t2[1][1] + t1[0][2]*t2[2][1] + t1[0][3]*t2[3][1];
  producto[0][2] = t1[0][0]*t2[0][2] + t1[0][1]*t2[1][2] + t1[0][2]*t2[2][2] + t1[0][3]*t2[3][2];
  producto[0][3] = t1[0][0]*t2[0][3] + t1[0][1]*t2[1][3] + t1[0][2]*t2[2][3] + t1[0][3]*t2[3][3];
  producto[1][0] = t1[1][0]*t2[0][0] + t1[1][1]*t2[1][0] + t1[1][2]*t2[2][0] + t1[1][3]*t2[3][0];
  producto[1][1] = t1[1][0]*t2[0][1] + t1[1][1]*t2[1][1] + t1[1][2]*t2[2][1] + t1[1][3]*t2[3][1];
  producto[1][2] = t1[1][0]*t2[0][2] + t1[1][1]*t2[1][2] + t1[1][2]*t2[2][2] + t1[1][3]*t2[3][2];
  producto[1][3] = t1[1][0]*t2[0][3] + t1[1][1]*t2[1][3] + t1[1][2]*t2[2][3] + t1[1][3]*t2[3][3];
  producto[2][0] = t1[2][0]*t2[0][0] + t1[2][1]*t2[1][0] + t1[2][2]*t2[2][0] + t1[2][3]*t2[3][0];
  producto[2][1] = t1[2][0]*t2[0][1] + t1[2][1]*t2[1][1] + t1[2][2]*t2[2][1] + t1[2][3]*t2[3][1];
  producto[2][2] = t1[2][0]*t2[0][2] + t1[2][1]*t2[1][2] + t1[2][2]*t2[2][2] + t1[2][3]*t2[3][2];
  producto[2][3] = t1[2][0]*t2[0][3] + t1[2][1]*t2[1][3] + t1[2][2]*t2[2][3] + t1[2][3]*t2[3][3];
  producto[3][0] = t1[3][0]*t2[0][0] + t1[3][1]*t2[1][0] + t1[3][2]*t2[2][0] + t1[3][3]*t2[3][0];
  producto[3][1] = t1[3][0]*t2[0][1] + t1[3][1]*t2[1][1] + t1[3][2]*t2[2][1] + t1[3][3]*t2[3][1];
  producto[3][2] = t1[3][0]*t2[0][2] + t1[3][1]*t2[1][2] + t1[3][2]*t2[2][2] + t1[3][3]*t2[3][2];
  producto[3][3] = t1[3][0]*t2[0][3] + t1[3][1]*t2[1][3] + t1[3][2]*t2[2][3] + t1[3][3]*t2[3][3];
  return producto;
}

class PlanetaryStation{
  private:
    float inclination, azimuth;
    Sphere planeta;
  public:
    PlanetaryStation() {}
    PlanetaryStation(Sphere sph, float incl, float azi){
      planeta = sph;
      inclination = incl;
      azimuth = azi;
    }
  };

  #endif
