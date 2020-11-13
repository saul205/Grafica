#ifndef SENSOR_H
#define SENSOR_H

#include "ConcurrentBoundedQueue.h"
#include "Transformation.h"
#include "Plane.h"
#include "Ray.h"
#include "Image.h"
#include "LightSource.h"
#include <memory>
#include <chrono>
#include <thread>
#include <random>
#include <functional>

const int sizeCuadrante = 30;

void lanzarRayosParalelizado(Image& newImagen, ConcurrentBoundedQueue& cbq, int antiAliasing, const BoundingVolume &scene, 
                        const float pixelSize, float centrarEnElPlanoW, float centrarEnElPlanoH, float planeW, DotDir oLocal,
                        DotDir oMundo, Transformation localAMundo){

    std::uniform_real_distribution<float> dist(0.0, pixelSize);
    std::default_random_engine gen;
    auto random = std::bind(dist, gen);

    std::uniform_real_distribution<float> dist2(0.0, 1);
    std::default_random_engine gen2;
    auto roussianRoulette = std::bind(dist2, gen2)

    DotDir planePoint, dir;
    cuadrante limites; 
    float intersecciones = 0;
    while(cbq.dequeue(limites)){

        for(int i = limites.minXlimit; i < limites.maxXlimit; ++i){
            for(int j = limites.minYlimit; j < limites.maxYlimit; ++j){

                float red = 0.0, green = 0.0, blue = 0.0;
                for(int z = 0; z < antiAliasing; ++z){

                    float h = random();
                    float w = random();

                    // Como f = 1 la tercera componente es fija
                    planePoint.setDotDir(pixelSize * i - centrarEnElPlanoW + w, - pixelSize * j + centrarEnElPlanoH - h, 1, 1);
                    dir = planePoint - oLocal;

                    DotDir dirMundo = localAMundo*dir;

                    dirMundo = normalization(dirMundo);
                    Ray rayoMundo( oMundo, dirMundo);
                  
                    std::shared_ptr<Figure> minTObject;
                    DotDir interseccion;
                    bool intersecta = scene.intersect(rayoMundo, minTObject, interseccion, intersecciones);

                    //--------Path Tracing---------
                    rgb luz(1, 1, 1);
                    bool bounce = false;
                    while(intersecta){

                        //Ruleta Rusa
                        float pk = minTObject->material.kd.max();
                        float ps = minTObject->material.ks.max();
                        float pt = minTObject->material.kt.max();

                        float maxes = pk + ps + pt;
                        pk /= maxes;
                        ps /= maxes;
                        pt /= maxes;

                        if(bounce){
                            pk *= 0.9;
                            ps *= 0.9;
                            pt *= 0.9;
                        }

                        DotDir base[3];
                        minTObject->getBase(interseccion, base);
                        
                        Transformation mundoALocal;
                        mundoALocal.changeBase(base[0], base[1], base[2], interseccion);
                        Transformation localAMundo = inverse(t);

                        DotDir wi;
                        float p = roussianRoulette();
                        if(p < pk){//Difuso
                            float i = roussianRoulette();
                            float azimuth = i * 2 * pi;

                            i = roussianRoulette();
                            float inclination = acos(sqrt(1 - i));
                            wi = DotDir(sin(inclination)*cos(azimuth), cos(inclination), sin(inclination)*sin(azimuth), 0);
                            wi = localAMundo * wi;

                        }else if(p < pk + ps){ // Specular
                            // wr = wi
                            DotDir wo = localAMundo * rayoMundo;
                            //Reflexión perfecta
                            wi = wo - 2.0f * base[1] * dotProduct(wo, base[1]);
                        }else if(p < pk + ps + pt){ // Refraccion
                            // Snell's Law
                        }else{ // Absorcion
                            // 0
                        }
                        //Lanzar nuevo rayo
                        
                        Ray newRay(interseccion, wi);
                        intersecta = scene.intersect(newRay, minTObject, interseccion, intersecciones);

                        //Calcular BRDF

                        //luz *= fr(x, wi, wo) * |n * wi| / p(wi)

                        //Parar si no intersecta, absorbe -> luz del camino = 0
                        //Si objeto emisor -> luz con valor
                        bounce = true;
                    }
                    //hacer algo con la luz
                    //--------Path Tracing---------
                }  

                red /= (float) antiAliasing;
                green /= (float) antiAliasing;
                blue /= (float) antiAliasing;
                newImagen.setRGB(i + j * planeW, rgb(red, green, blue));
            }
        }
    }
    //cout << "Intersecciones: " << intersecciones << endl;
}

class Sensor{
    private:

        DotDir lMundo, uMundo, fMundo, oMundo;
        DotDir lLocal, uLocal, fLocal, oLocal;
        Transformation localAMundo;
        float planeW, planeH;
        float pixelSize, centrarEnElPlanoW, centrarEnElPlanoH;
        Plane projectionPlane;

    public:

        Sensor(){}

        Sensor(DotDir _l, DotDir _u, DotDir _f, DotDir _o, float planeWidth, float planeHeight) :
                lMundo(_l), uMundo(_u), fMundo(_f), oMundo(_o), planeW(planeWidth), planeH(planeHeight) {

            lLocal = DotDir(1, 0, 0, 0);
            uLocal = DotDir(0, 1, 0, 0);
            fLocal = DotDir(0, 0, 1, 0);
            oLocal = DotDir(0, 0, 0, 1);

            localAMundo.changeBase(lMundo, uMundo, fMundo, oMundo);
            // No se está usando el prjection plane
            projectionPlane.setNormal(fMundo);
        }

        // nThreads valor mínimo 1
        void lanzarRayos(const BoundingVolume &scene,  Image& imagen, int antiAliasing, const int nThreads = 1){
            
            if(planeH <= planeW){
                pixelSize = 2 / planeH;
            }else{
                pixelSize = 2 / planeW;
            }
            
            ConcurrentBoundedQueue cbq;
            for(int i = 0; i < planeW; i = i + sizeCuadrante){
                for(int j = 0; j < planeH; j = j + sizeCuadrante){
                    if(i + sizeCuadrante <= planeW && j + sizeCuadrante <= planeH){
                        cbq.enqueue(cuadrante(cuadrante(i, i + sizeCuadrante, j, j + sizeCuadrante)));
                    } else if(i + sizeCuadrante > planeW && j + sizeCuadrante <= planeH){
                        cbq.enqueue(cuadrante(cuadrante(i, planeW, j, j + sizeCuadrante)));
                    } else if(i + sizeCuadrante <= planeW && j + sizeCuadrante > planeH){
                        cbq.enqueue(cuadrante(cuadrante(i, i + sizeCuadrante, j, planeH)));
                    } else {
                        cbq.enqueue(cuadrante(cuadrante(i, planeW, j, planeH)));
                    }               
                }
            }

            centrarEnElPlanoW = pixelSize * planeW / 2;
            centrarEnElPlanoH = pixelSize * planeH / 2;

            std::vector<std::thread> th(nThreads);
            auto start = chrono::steady_clock::now();

            for(int i = 0; i < nThreads; ++i){
                th[i] = thread(&lanzarRayosParalelizado, std::ref(imagen), std::ref(cbq), antiAliasing, scene, pixelSize, centrarEnElPlanoW, centrarEnElPlanoH, planeW, oLocal, oMundo, localAMundo);
            }
            
            for(int i = 0; i < nThreads; ++i){
                th[i].join();
            }

            auto end = chrono::steady_clock::now();
            cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;

        }
};

#endif