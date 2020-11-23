#ifndef SENSOR_H
#define SENSOR_H

#include "ConcurrentQueue.h"
#include "Transformation.h"
#include "Plane.h"
#include "Ray.h"
#include "Sphere.h"
#include "Image.h"
#include "LightSource.h"
#include "BoundingVolume.h"
#include <memory>
#include <chrono>
#include <thread>
#include <random>
#include <functional>

const int sizeCuadrante = 5;
const float fov = 1;
const float airRefraction = 1.000293;

// Obtiene las probabilidades correspondientes al comportamiento del material
void getMaterialProbabilities(shared_ptr<Figure> f, float& pk, float& ps, float& pt, bool bounce);

// Obtiene un rayo a partir del muestreo del coseno
DotDir getCosineSamplingRay(float r1, float r2);

// Obtiene el rayo especular a partir de la normal y el incidente
DotDir getSpecularRay(DotDir normal, DotDir wo);

// Obtiene un rayo refractado
DotDir getRefractedRay(DotDir incidente, DotDir normal, float refractionIndex);

void lanzarRayosParalelizado(Image& newImagen, ConcurrentQueue& cbq, int antiAliasing, const BoundingVolume &scene, 
                            vector<LightSource> luces, const float pixelSizeX, const float pixelSizeY, float centrarEnElPlanoW, 
                            float centrarEnElPlanoH, float planeW, Transformation localAMundo);

class Sensor{
    private:

        Transformation localAMundo;
        float planeW, planeH;
        float pixelSize, centrarEnElPlanoW, centrarEnElPlanoH;

    public:

        Sensor(){}
        
        // Calcula la base de la camara en función del fov, aspect ratio y target
        // Se calcula sabiendo el valor del vector front
        Sensor(float fov, float aspect, DotDir target, float planeWidth, float planeHeight, DotDir _f = DotDir(0,0,1,0));

        // Paraleliza el proceso de pathtracing, invocando nThread threads
        void lanzarRayos(const BoundingVolume &scene, vector<LightSource> luces, Image& imagen, int antiAliasing, const int nThreads = 1);

};

#endif