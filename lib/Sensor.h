#ifndef SENSOR_H
#define SENSOR_H

#include "ConcurrentBoundedQueue.h"
#include "Transformation.h"
#include "Plane.h"
#include "Ray.h"
#include "Sphere.h"
#include "Image.h"
#include "LightSource.h"
#include <memory>
#include <chrono>
#include <thread>
#include <random>
#include <functional>

const int sizeCuadrante = 5;
const float fov = 1;

// Lleva a cabo el proceso de pathtracing
void lanzarRayosParalelizado(Image& newImagen, ConcurrentBoundedQueue& cbq, int antiAliasing, const BoundingVolume &scene, vector<LightSource> luces,
                        const float pixelSizeX, const float pixelSizeY, float centrarEnElPlanoW, float centrarEnElPlanoH, float planeW, Transformation camAMundo){

    std::uniform_int_distribution<int> dist(0, luces.size()-1);
    std::uniform_real_distribution<float> dist2(0.0, 1);
    std::default_random_engine gen, gen2;
    auto lucesRoussianRoulette = std::bind(dist, gen);
    auto roussianRoulette = std::bind(dist2, gen2);

    cuadrante limites; 
    float intersecciones = 0;
    while(cbq.dequeue(limites)){

        for(int i = limites.minXlimit; i < limites.maxXlimit; ++i){
            for(int j = limites.minYlimit; j < limites.maxYlimit; ++j){

                rgb emisionFinal(0, 0, 0);
                for(int z = 0; z < antiAliasing; ++z){
                    float h = roussianRoulette()*pixelSizeX;
                    float w = roussianRoulette()*pixelSizeY;

                    // f = 1 la tercera componente es fija
                    DotDir planePoint(- (pixelSizeX * i - centrarEnElPlanoW + w), - pixelSizeY * j + centrarEnElPlanoH - h, 1, 1);

                    DotDir oLocal = DotDir(0,0,0,1), interseccion;
                    Ray rayoMundo( camAMundo*oLocal, normalization(camAMundo*(planePoint-oLocal)));
                    std::shared_ptr<Figure> minTObject;
                    
                    //--------Path Tracing, camino para un rayo---------
                    rgb emisionAcumulada(1, 1, 1), emisionFinalRayo(0, 0, 0);
                    Ray rayoMundoRebotes = rayoMundo;
                    // Marcan si es el primer rebote y si ha intersectado, respectivamente 
                    bool bounce = false, intersecta = true;
    
                    while(intersecta) {
                        
                        intersecta = scene.intersect(rayoMundoRebotes, minTObject, interseccion, intersecciones);

                        if(intersecta && minTObject->hasEmission()) {
                            emisionFinalRayo = emisionFinalRayo + minTObject->getEmission(interseccion);
                            intersecta = false;
                        } else if(intersecta){

                            // Cambia la dirección de la normal si es necesario 
                            DotDir base[3], wi;
                            minTObject->getBase(interseccion, base[0], base[1], base[2]);
                            if(dotProduct(base[2], rayoMundoRebotes.getDir()) > 0 )
                                base[2] = -base[2];

                            // Calcula las transformaciones entre mundo y local                         
                            Transformation mundoToLocal, localToMundo;
                            localToMundo.changeBase(base[0], base[1], base[2], interseccion);
                            mundoToLocal = inverse(localToMundo);

                            // Calcula la normal y el rayo en coordenadas locales a la intersección
                            DotDir incide = normalization(mundoToLocal*rayoMundoRebotes.getDir());
                            DotDir normalLocal = normalization(mundoToLocal*base[2]);

                            //Ruleta Rusa
                            float pk, ps, pt, p = roussianRoulette();
                            getMaterialProbabilities(minTObject, incide, normalLocal, pk, ps, pt, bounce);

                            if(p < pk){                         //Difuso

                                wi = getCosineSamplingRay(roussianRoulette(), roussianRoulette());
                                float coseno = abs(dotProduct(normalLocal, wi));
                                emisionAcumulada = emisionAcumulada*(minTObject->getDifRgb()*(coseno/(pi*pk)));
                            }else if(p < pk + ps){              // Especular

                                wi = getSpecularRay(normalLocal, incide);
                                emisionAcumulada = emisionAcumulada*(minTObject->getSpecRgb() / ps);
                            }else if(p < pk + ps + pt){         // Refraccion

                                wi = getRefractedRay(incide, normalLocal, minTObject->refractionIndex);
                                emisionAcumulada = emisionAcumulada*(minTObject->getRefRgb() / pt);
                            }
                            else{                               // Absorcion
                                intersecta = false;
                            }

                            wi = localToMundo * wi;
                            Ray newRay(interseccion, normalization(wi));
                            rayoMundoRebotes = newRay;
                            
                            // Next event estimation
                            if(luces.size() > 0){
                                int light = lucesRoussianRoulette();
                                DotDir shadowRay = luces[light].getPosition() - interseccion;
                                Ray newShadowRay(interseccion, normalization(shadowRay));
                                DotDir punto = interseccion;

                                bool luzIntersecta = scene.intersect(newShadowRay, minTObject, interseccion, intersecciones);
                                if(!luzIntersecta || (interseccion - punto).mod() > shadowRay.mod())
                                    emisionFinalRayo = emisionFinalRayo + emisionAcumulada*(luces[light].getEmission() / (shadowRay.mod() * shadowRay.mod()));
                            }
                        }
                        bounce = true;
                    }
                    emisionFinal = emisionFinal + emisionFinalRayo * emisionAcumulada;
                }  
                emisionFinal = emisionFinal / (float) antiAliasing;
                newImagen.setRGB(i + j * planeW, emisionFinal);
            }
        }
    }
}

class Sensor{
    private:

        Transformation localAMundo;
        float planeW, planeH;
        float pixelSize, centrarEnElPlanoW, centrarEnElPlanoH;

    public:

        Sensor(){}
        
        // Calcula la base de la camara en función del fov, aspect ratio y target
        // Se calcula sabiendo el valor del vector front
        Sensor(float fov, float aspect, DotDir target, float planeWidth, float planeHeight, DotDir _f = DotDir(0,0,1,0)) :
                planeW(planeWidth), planeH(planeHeight) {

            float ufRatio = tan(pi * 30 / (2.0f * 180));
            cout << ufRatio << endl;
            
            DotDir _u = DotDir(0,ufRatio * _f.mod(),0,0);
            DotDir _l = DotDir(aspect * _u.mod(),0,0,0);
            DotDir _o = target - _f;

            cout << _l.toString() << endl;
            cout << _u.toString() << endl;
            cout << _f.toString() << endl;
            cout << _o.toString() << endl;

            localAMundo.changeBase(_l, _u, _f, _o);
        }

        // nThreads valor mínimo 1
        void lanzarRayos(const BoundingVolume &scene, vector<LightSource> luces, Image& imagen, int antiAliasing, const int nThreads = 1){
            
            float pixelSizeX = 2 / planeW;
            float pixelSizeY = 2 / planeH;

            
            ConcurrentBoundedQueue cbq;
            for(int i = 0; i < planeW; i = i + sizeCuadrante){
                for(int j = 0; j < planeH; j = j + sizeCuadrante){
                    if(i + sizeCuadrante <= planeW && j + sizeCuadrante <= planeH){
                        cbq.enqueue(cuadrante(i, i + sizeCuadrante, j, j + sizeCuadrante));
                    } else if(i + sizeCuadrante > planeW && j + sizeCuadrante <= planeH){
                        cbq.enqueue(cuadrante(i, planeW, j, j + sizeCuadrante));
                    } else if(i + sizeCuadrante <= planeW && j + sizeCuadrante > planeH){
                        cbq.enqueue(cuadrante(i, i + sizeCuadrante, j, planeH));
                    } else {
                        cbq.enqueue(cuadrante(i, planeW, j, planeH));
                    }               
                }
            }

            centrarEnElPlanoW = pixelSizeX * planeW / 2;
            centrarEnElPlanoH = pixelSizeY * planeH / 2;

            std::vector<std::thread> th(nThreads);
            auto start = chrono::steady_clock::now();

            for(int i = 0; i < nThreads; ++i){
                th[i] = thread(&lanzarRayosParalelizado, std::ref(imagen), std::ref(cbq), antiAliasing, scene, luces, pixelSizeX, pixelSizeY, centrarEnElPlanoW, centrarEnElPlanoH, planeW, localAMundo);
            }
            for(int i = 0; i < nThreads; ++i){
                th[i].join();
            }

            auto end = chrono::steady_clock::now();
            cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;

        }
};

#endif