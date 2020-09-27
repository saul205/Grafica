#ifndef SPHERE_H
#define SPHERE_H

#include "Transformation.h"

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

    // Construye una base que incluye como uno de sus ejes el axis de la esfera
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