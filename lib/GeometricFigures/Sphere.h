#ifndef SPHERE_H
#define SPHERE_H

#include "Transformation.h"
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
    Sphere(const DotDir& center, const DotDir& axis, const DotDir& city);

    // Si no instersecta o está detrás de la cámara devuelve falso y no modifica t,
    // en caso contrario devuelve cierto y t²|d|² + 2td*(o-c) + |o-c|² - r² = 0
    // El rayo, si normalizado, |d|² = 1, se despeja t y se guarda la solución de la ecuación
    // en t. Guarda la solución que representa el punto más cercano
    bool intersects(Ray ray, float& t, DotDir& p) override;

    rgb getTexture(const DotDir& interseccion) override;

    //--------------------GETTERS-------------------------

    // Construye una base que incluye como uno de sus ejes el axis de la esfera
    /*
          El axis es el eje Y en la función getBase
          El eje X es perpendicular a city - center y el axis
          El eje Z es perpendicular a Y y a X
    */
    void getBase(DotDir base[3]);

    void getBase(DotDir interseccion, DotDir& base0, DotDir& base1, DotDir& base2) override;

    DotDir getAxis();

    DotDir getCity();

    DotDir getCenter() override;

    float getRadius();

    BoundingBox getBound();

};

// DevuelYxyve cierto si y solo si el radio definido por el eje del planeta,
// el cual corresponde a su diámetro (debe ser una dirección),
// y la distancia entre el centro de la esfera y la ciudad de referencia
// (ambos deben ser puntos) difieren en menos de 10e-6.
bool checkRadius(DotDir axis, DotDir center, DotDir city);


class PlanetaryStation{
  private:

    float inclination, azimuth;
    Sphere planet;

    // En UCS
    DotDir position;

    // Base en la estación en UCS
    // 0 = tangente latitudinal
    // 1 = normal
    // 2 = tangente longitudinal
    DotDir basePosition[3];

    // Base del centro del planeta en UCS
    // 0 = segundo eje calculado
    // 1 = axis
    // 2 = primer eje calculado
    DotDir baseCentro[3];

  public:

    friend void conection(const PlanetaryStation& origen, const PlanetaryStation& destino);

    PlanetaryStation(Sphere sph, float incl, float azi);
};

// Función encargada de llevar a cabo la conexión entre los planetas descrita
// en la práctica 1 de la asignatura Informática Gráfica
void conection(const PlanetaryStation& origen, const PlanetaryStation& destino);

#endif