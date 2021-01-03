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

const int sizeCuadrante = 1;
const float fov = 1;

void lanzarRayosParalelizado(Image& newImagen, ConcurrentBoundedQueue& cbq, int antiAliasing, const BoundingVolume &scene, vector<LightSource> luces,
                        const float pixelSizeX, const float pixelSizeY, float centrarEnElPlanoW, float centrarEnElPlanoH, float planeW, Transformation localAMundo){

    std::uniform_int_distribution<int> dist(0, luces.size()-1);
    std::default_random_engine gen;
    auto lucesRoussianRoulette = std::bind(dist, gen);

    std::uniform_real_distribution<float> dist2(0.f, 1.f);
    std::default_random_engine gen2;
    auto roussianRoulette = std::bind(dist2, gen2);

    DotDir planePoint, dir;
    cuadrante limites; 
    float intersecciones = 0;
    while(cbq.dequeue(limites)){
        for(int i = limites.minXlimit; i < limites.maxXlimit; ++i){
            for(int j = limites.minYlimit; j < limites.maxYlimit; ++j){

                rgb emisionFinal(0, 0, 0);
                for(int z = 0; z < antiAliasing; ++z){
                    float h = roussianRoulette()*pixelSizeY;
                    float w = roussianRoulette()*pixelSizeX;

                    DotDir oLocal(0,0,0,1);
                    // Como f = 1 la tercera componente es fija
                    planePoint.setDotDir(- (pixelSizeX * i - centrarEnElPlanoW + w), - pixelSizeY * j + centrarEnElPlanoH - h, 1, 1);

                    Ray rayoMundo( localAMundo*oLocal, normalization(localAMundo*(planePoint - oLocal)));
                  
                    std::shared_ptr<Figure> minTObject;
                    DotDir interseccion;

                    //--------Path Tracing---------
                    rgb emisionAcumulada(1, 1, 1);
                    rgb emisionFinalRayo(0, 0, 0);
                    Ray rayoMundoRebotes = rayoMundo;
                    bool bounce = false;
                    bool intersecta = true;
                    while(intersecta) {
                        
                        intersecta = scene.intersect(rayoMundoRebotes, minTObject, interseccion, intersecciones);

                        if(intersecta && minTObject->hasEmission()) {

                            emisionFinalRayo = emisionFinalRayo + minTObject->getEmission(interseccion);
                            intersecta = false;

                        } else if(intersecta){

                            DotDir base[3], wi;
                            minTObject->getBase(interseccion, base[0], base[1], base[2]);

                            //Ruleta Rusa
                            float pk, ps, pt, p = roussianRoulette();
                            getMaterialProbabilities(minTObject, rayoMundoRebotes.getDir(), base[2], pk, ps, pt, bounce);

                            if(p < pk){                 //Difuso

                                if(dotProduct(base[2], rayoMundoRebotes.getDir()) > 0)
                                    base[2] = -base[2];

                                Transformation localAMundo;
                                localAMundo.changeBase(base[0], base[1], base[2], interseccion);
                                wi = getCosineSamplingRay(roussianRoulette(), roussianRoulette(), localAMundo);
                                emisionAcumulada = emisionAcumulada*(minTObject->getDifRgb(interseccion)/pk);

                                // Next event estimation
                                if(luces.size() > 0){
                                    int light = lucesRoussianRoulette();
                                    DotDir shadowRay = luces[light].getPosition() - interseccion;
                                    Ray newShadowRay(interseccion, normalization(shadowRay));
                                    DotDir punto;

                                    bool luzIntersecta = scene.intersect(newShadowRay, minTObject, punto, intersecciones);
                                    if(!luzIntersecta || (punto - interseccion).mod() > shadowRay.mod()){
                                        float evaluar = luces.size() * fabs(dotProduct(base[2], shadowRay / shadowRay.mod())) / pi;
                                        emisionFinal = emisionFinal + emisionAcumulada*evaluar*(luces[light].getEmission() / (shadowRay.mod() * shadowRay.mod()));
                                    }
                                }
 
                            }else if(p < pk + ps){      // Specular

                                wi = getSpecularRay(base[2], rayoMundoRebotes.getDir());
                                emisionAcumulada = emisionAcumulada*(minTObject->getSpecRgb() / ps);
                            }else if(p < pk + ps + pt){ // Refraccion

                                wi = getRefractedRay(rayoMundoRebotes.getDir(), base[2], minTObject->refractionIndex);
                                emisionAcumulada = emisionAcumulada*(minTObject->getRefRgb() / pt);
                            }else{                      // Absorcion
                                intersecta = false;
                            } 
                            rayoMundoRebotes = Ray(interseccion, normalization(wi));
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

        DotDir lMundo, uMundo, fMundo, oMundo;
        DotDir oLocal;
        Transformation localAMundo;
        float planeW, planeH;
        float centrarEnElPlanoW, centrarEnElPlanoH;

    public:

        Sensor(){}

        Sensor(float fov, float aspect, DotDir target, float planeWidth, float planeHeight, DotDir _f = DotDir(0,0,1,0)) :
                planeW(planeWidth), planeH(planeHeight) {

            float ufRatio = tan(pi * fov / (2.0f * 180));
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
        void lanzarRayos(const BoundingVolume &scene,  vector<LightSource> luces, Image& imagen, int antiAliasing, const int nThreads = 1){
            
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

            centrarEnElPlanoW = pixelSizeX * planeW / 2.0f;
            centrarEnElPlanoH = pixelSizeY * planeH / 2.0f;

            std::vector<std::thread> th(nThreads);

            cout << luces.size() << endl;
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