#ifndef SPHERE_H
#define SPHERE_H

#include "Transformation.h"
#include "Rgb.h"
#include "Figure.h"

const double pi = 3.141592653589793;

class Sphere : public Figure{
  
  private:
    // En UCS
    DotDir sphereCenter, sphereAxis, sphereCity;
    float radius;

    Transformation UCSToLocalTransformation;
    DotDir centerLocal;

  public:

    Sphere() {}

    // Los parámetros center, axis y city deben cumplir
    // checkRadius(center, axis, city)
    Sphere(const DotDir& center, const DotDir& axis, const DotDir& city){
      sphereCenter = center;
      sphereAxis = axis;
      sphereCity = city;
      radius = axis.mod()/2;

      DotDir baseUCS[3];
      getBase(baseUCS);

      UCSToLocalTransformation.changeBase(baseUCS[0], baseUCS[1], baseUCS[2], center);
      UCSToLocalTransformation = inverse(UCSToLocalTransformation);
      centerLocal = UCSToLocalTransformation*center;
    }

    // Si no instersecta o está detrás de la cámara devuelve falso y no modifica t,
    // en caso contrario devuelve cierto y t²|d|² + 2td*(o-c) + |o-c|² - r² = 0
    // El rayo, si normalizado, |d|² = 1, se despeja t y se guarda la solución de la ecuación
    // en t. Guarda la solución que representa el punto más cercano
    bool instersects(Ray ray, float& t, DotDir& p) override {
        DotDir diff = ray.getOrigen() - sphereCenter;
        float modDiff = diff.mod();
        
        float a = 1;
        float b = 2.0f*dotProduct(ray.getDir(), diff);
        float c = (modDiff*modDiff) - (radius*radius);
        float doSquareRoot = b*b - 4.0f*a*c;

        if(doSquareRoot < 0) {         // Sin soluciones
          return false;

        } else if(doSquareRoot == 0) { // Una solución, t es tangente a la esfera
          float answer = (-b)/(2.0f);  // a = 1 y la raíz es 0
          if(answer > 0){
            t = answer;

            p = ray.getOrigen() + t * ray.getDir();
            return true;
          } else {
            return false;
          }

        } else {

          float squareRoot = sqrt(doSquareRoot);
          float sol1 = ((-b) + squareRoot) / 2.0f;
          float sol2 = ((-b) - squareRoot) / 2.0f;
          if(sol1 > 0 && sol2 > 0){
            t = (sol1 < sol2) ? sol1 : sol2;
            //cout << t << endl;
            p = ray.getOrigen() + t * ray.getDir();
            return true;
          } else {                    // La cámara está dentro de la esfera
            return false;
          }
        }
        
    }

    rgb getTexture(DotDir& interseccion){

      DotDir interseccionLocal = UCSToLocalTransformation*interseccion;
      // cout << interseccionLocal.toString()<< endl;

      float azimuth = atan2(interseccionLocal.getZ() - centerLocal.getZ() , interseccionLocal.getX() - centerLocal.getX());
      float u = (azimuth + pi) / (2*pi);
      
      // Inclinación DESDE EL POLO NORTE, entre 0 y pi
      float inclination = acos(interseccionLocal.getY() / radius);
      float v = inclination / pi;

      u = u*textura.getWidth();
      v = v*textura.getHeight();

      // cout << interseccionLocal.getX() << "  " << radius*cos(inclination)*sin(azimuth) << endl;
      // cout << interseccionLocal.getY() << "  " << -radius*cos(azimuth) << endl;
      // cout << interseccionLocal.getZ() << "  " << -radius*sin(inclination)*sin(azimuth) << endl;

      rgb dev = textura.getRGB(v, u);
      return dev;

    }

    //--------------------GETTERS-------------------------

    // Construye una base que incluye como uno de sus ejes el axis de la esfera
    /*
          El axis es el eje Y en la función getBase
          El eje X es perpendicular a city - center y el axis
          El eje Z es perpendicular a Y y a X
    */
    void getBase(DotDir base[3]){
      
      base[1] = sphereAxis;
      base[2] = crossProduct(sphereCity - sphereCenter, sphereAxis);
      base[0] = crossProduct(base[1], base[2]);

      if(base[2].mod() > 0)
        base[2] = normalization(base[2]);
      if(base[0].mod() > 0)
        base[0] = normalization(base[0]);
      if(base[1].mod() > 0)
        base[1] = normalization(base[1]);
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

// DevuelYxyve cierto si y solo si el radio definido por el eje del planeta,
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
    Sphere planet;

    // En UCS
    DotDir position;

    // Base en la estación en UCS
    // 0 = tangente longitudinal
    // 1 = normal
    // 2 = tangente latitudinal
    DotDir basePosition[3];

    // Base del centro del planeta en UCS
    // 0 = primer eje calculado
    // 1 = axis
    // 2 = segundo eje calculado
    DotDir baseCentro[3];

  public:

    friend void conection(const PlanetaryStation& origen, const PlanetaryStation& destino);

    PlanetaryStation(Sphere sph, float incl, float azi){
      
      planet = sph;
      inclination = incl;
      azimuth = azi;

      // 0 = primer eje calculado
      // 1 = axis
      // 2 = segundo eje calculado
      sph.getBase(baseCentro);

      DotDir extremoAxis(0,planet.getRadius(),0,1);
      // Rotamos sobre el segundo de los ejes calculado
      Transformation rotacionInclination;
      // Rotamos sobre el tercero de los ejes calculado (axis)
      Transformation rotacionAzimuth;
      rotacionInclination.rotationZ(inclination);
      rotacionAzimuth.rotationY(azimuth);
      position = rotacionInclination*extremoAxis;
      position = rotacionAzimuth*position;

      cout << "Posicion Local: " << position.toString() << endl << endl;

      // Pasar la posición de local a UCS
      Transformation localUCS;
      localUCS.changeBase(baseCentro[0], baseCentro[1], baseCentro[2], sph.getCenter());
      Transformation ucsLocal = inverse(localUCS);

      cout << "Base UCS a local" << endl << ucsLocal.toString() << endl;
      cout << "Base local a UCS" << endl << localUCS.toString() << endl;

      position = localUCS*position;

      cout << "Posicion UCS: " << position.toString() << endl << endl;

      // Calculo de la normal y las tangentes
      // 0 = tangente longitudinal
      // 1 = normal
      // 2 = tangente latitudinal
      basePosition[1] = position - planet.getCenter();
      basePosition[2] = crossProduct(baseCentro[1], basePosition[1]);
      basePosition[0] = crossProduct(basePosition[1], basePosition[2]);

      basePosition[2] = normalization(basePosition[2]);
      basePosition[0] = normalization(basePosition[0]);
      basePosition[1] = normalization(basePosition[1]);
    }
  };

  void conection(const PlanetaryStation& origen, const PlanetaryStation& destino){

      // Obtengo la dirección entre estaciones en ambas direcciones 

      cout << "Posiciones " << destino.position.toString() << " | " << origen.position.toString() << endl << endl;
      DotDir dirUCS = destino.position - origen.position;

      Transformation ucsToPositionOrigen;
      ucsToPositionOrigen.changeBase(origen.basePosition[0], origen.basePosition[1], origen.basePosition[2], origen.position);
      ucsToPositionOrigen = inverse(ucsToPositionOrigen);

      cout << "Base Ucs a posicion Origen" << endl << ucsToPositionOrigen.toString() << endl << endl;

      DotDir origenDir = ucsToPositionOrigen*dirUCS;
      Transformation ucsToPositionDestino;
      ucsToPositionDestino.changeBase(destino.basePosition[0], destino.basePosition[1], destino.basePosition[2], destino.position);
      ucsToPositionDestino = inverse(ucsToPositionDestino);
      
      cout << "Base Ucs a posicion Destino" << endl << ucsToPositionDestino.toString() << endl << endl;

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