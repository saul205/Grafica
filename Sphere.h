#ifndef SPHERE_H
#define SPHERE_H

#include "Transformation.h"
#include "math.h"

class Sphere{
  
  private:
    // En UCS
    DotDir sphereCenter, sphereAxis, sphereCity;
    float radius;
  public:

    Sphere(){}
    // Los parámetros center, axis y city deben cumplir
    // checkRadius(center, axis, city)
    Sphere(const DotDir& center, const DotDir& axis, const DotDir& city){
      sphereCenter = center;
      sphereAxis = axis;
      sphereCity = city;
      radius = axis.mod()/2;
    }

    // Devuelve cierto si y solo si el radio definido por el eje del planeta,
    // el cual corresponde a su diámetro (debe ser una dirección),
    // y la distancia entre el centro de la esfera y la ciudad de referencia
    // (ambos deben ser puntos) difieren en menos de 10e-6.
    bool checkRadius(DotDir axis, DotDir center, DotDir city){
      DotDir radius = center - city;
      return (abs(radius.mod() - 0.5*(axis.mod())) < 0.000001) ? true : false;
    }

    // Construye una base que incluye como uno de sus ejes el axis de la esfera
    void getBase(DotDir base[3]){
      base[0] = crossProduct(sphereAxis, sphereCity - sphereCenter);
      base[1] = crossProduct(base[0], base[1]);
      base[2] = sphereAxis;
    }

    DotDir getAxis(){
      return sphereAxis;
    }

    DotDir getCity(){
      return sphereCity;
    }

    DotDir getCenter(){
      return sphereCenter;
    } 

    float getRadius(){
      return radius;
    } 
};

class PlanetaryStation{
  private:

    float inclination, azimuth;
    Sphere planet;

    // En UCS
    DotDir position;

    // Base en la estación en UCS
    // 0 = tangente longitudinal
    // 1 = tangente latitudinal
    // 2 = normal
    DotDir basePosition[3];

    // Base del centro del planeta en UCS
    // 0 = primer eje calculado
    // 1 = segundo eje calculado
    // 2 = axis
    DotDir baseCentro[3];

  public:

    friend void conection(const PlanetaryStation& origen, const PlanetaryStation& destino);

    PlanetaryStation(Sphere sph, float incl, float azi){
      
      planet = sph;
      inclination = incl;
      azimuth = azi;

      // 0 = primer eje calculado
      // 1 = segundo eje calculado
      // 2 = axis
      sph.getBase(baseCentro);

      DotDir extremoAxis(0,0,planet.getRadius(),1);

      // Rotamos sobre el segundo de los ejes calculado
      Transformation rotacionInclination;
      // Rotamos sobre el tercero de los ejes calculado (axis)
      Transformation rotacionAzimuth;
      rotacionInclination.rotationY(inclination);
      rotacionAzimuth.rotationZ(azimuth);
      position = rotacionInclination*extremoAxis;
      position = rotacionAzimuth*position;

      // Pasar la posición de local a UCS
      Transformation ucsLocal;
      ucsLocal.changeBase(baseCentro[0], baseCentro[1], baseCentro[2], sph.getCenter());
      Transformation localUCS = inverse(ucsLocal);
      position = localUCS*position;

      // Calculo de la normal y las tangentes
      // 0 = tangente longitudinal
      // 1 = tangente latitudinal
      // 2 = normal
      basePosition[2] = position - planet.getCenter();
      basePosition[0] = crossProduct(baseCentro[2], basePosition[2]);
      basePosition[1] = crossProduct(basePosition[2], basePosition[0]);
    }
  };

  void conection(const PlanetaryStation& origen, const PlanetaryStation& destino){

      // Obtengo la dirección entre estaciones en ambas direcciones 
      DotDir dirUCS = destino.position - origen.position;
      Transformation ucsToPositionOrigen;
      ucsToPositionOrigen.changeBase(origen.basePosition[0], origen.basePosition[1], origen.basePosition[2], origen.position);
      DotDir origenDir = ucsToPositionOrigen*dirUCS;
      Transformation ucsToPositionDestino;
      ucsToPositionDestino.changeBase(destino.basePosition[0], destino.basePosition[1], destino.basePosition[2], destino.position);
      DotDir destinoDir = ucsToPositionDestino*dirUCS;

      if(origenDir.getZ() < 0){
        cout << "El lanzamiento desde la estación de origen no es posible." << endl;
      }
      if(destinoDir.getZ() > 0){
         cout << "La recepción en la estación destino no es posible." << endl;
      }

      cout << "Coordenadas de la conexión en UCS:                 " << dirUCS.toString() << endl;  
      cout << "Coordenadas de la conexión en coordenadas origen:  " << origenDir.toString() << endl;  
      cout << "Coordenadas de la conexión en coordenadas destino: " << destinoDir.toString() << endl;  
  }

#endif