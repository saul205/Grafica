#ifndef SENSOR_H
#define SENSOR_H

#include "DotDir.h"
#include "ConcurrentBoundedQueue.h"
#include "Transformation.h"
#include "Plane.h"
#include "Ray.h"
#include "Triangle.h"
#include "Image.h"
#include "LightSource.h"
#include <memory>
#include <chrono>
#include <thread>
#include <random>
#include <functional>

const int sizeCuadrante = 30;

void lanzarRayosParalelizado(Image& newImagen, ConcurrentBoundedQueue& cbq, int antiAliasing, vector<Figure*> const objetos, 
                        const float pixelSize, float centrarEnElPlanoW, float centrarEnElPlanoH, float planeW, DotDir oLocal,
                        DotDir oMundo, Transformation localAMundo){

    std::uniform_real_distribution<float> dist(0.0, pixelSize);
    std::default_random_engine gen;
    auto random = std::bind(dist, gen);

    int minTObject = -1;
    DotDir planePoint, dir;
    cuadrante limites; 

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

                    DotDir inters, interseccion;
                    float minT = INFINITY, newT = INFINITY;
                    for(int i = 0; i < objetos.size(); i++){
                        // Si no intersecta no se modifica newT
                        if(objetos[i]->intersects(rayoMundo, newT, inters)){
                            if(newT < minT){
                                minT = newT;
                                minTObject = i;
                                interseccion = inters;
                            }
                        }
                    }
                
                    if(minTObject >= 0){
                        rgb color = objetos[minTObject]->getEmission(interseccion);
                        red += color.r;
                        green += color.g;
                        blue += color.b;
                        minTObject = -1;
                    }
                }  

                red /= (float) antiAliasing;
                green /= (float) antiAliasing;
                blue /= (float) antiAliasing;
                newImagen.setRGB(i + j * planeW, rgb(red, green, blue));
            }
        }
    } 
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
        void lanzarRayos(vector<Figure*> objetos,  Image& imagen, int antiAliasing, const int nThreads = 1){
            
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
                th[i] = thread(&lanzarRayosParalelizado, std::ref(imagen), std::ref(cbq), antiAliasing, objetos, pixelSize, centrarEnElPlanoW, centrarEnElPlanoH, planeW, oLocal, oMundo, localAMundo);
            }
            
            for(int i = 0; i < nThreads; ++i){
                th[i].join();
            }

            auto end = chrono::steady_clock::now();
            cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;

        }
};

#endif