#ifndef esfera
#define esfera

#include<math.h>
#include<string>
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

DotDir dotProduct(const DotDir d1, const DotDir d2){
  return DotDir(d1.c[0]*d2.c[0], d1.c[1]*d2.c[1], d1.c[2]*d2.c[2], d1.c[3]*d2.c[3]);
}

DotDir crossProduct(const DotDir d1, const DotDir d2){
  return DotDir(d1.c[1]*d2.c[2] - d1.c[2]-d2.c[1], 
                d1.c[2]*d2.c[0] - d1.c[0]-d2.c[2], 
                d1.c[0]*d2.c[1] - d1.c[1]-d2.c[0], d1.c[3] * d2.c[3]);
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