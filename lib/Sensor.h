#ifndef SENSOR_H
#define SENSOR_H

#include "DotDir.h"
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


void lanzarRayosParalelizado(Image& newImagen, int wminlimit, int wmaxlimit, int hminlimit,
                        int hmaxlimit, int antiAliasing, vector<Figure*> const objetos, const float pixelSize,
                        float centrarEnElPlanoW, float centrarEnElPlanoH, float planeW, DotDir oLocal, DotDir oMundo,
                        Transformation localAMundo, int threadId){

    std::uniform_real_distribution<float> dist(0.0, pixelSize);
    std::default_random_engine gen;

    auto random = std::bind(dist, gen);

    int minTObject = -1;
    DotDir planePoint, dir;

    for(int i = wminlimit; i < wmaxlimit; ++i){
        for(int j = hminlimit; j < hmaxlimit; ++j){

            std::vector<rgb> color(antiAliasing);
            for(int z = 0; z < antiAliasing; ++z){

                float h = random();
                float w = random();


                // Como f = 1 la tercera componente es fija
                planePoint.setDotDir(pixelSize * i - centrarEnElPlanoW + w, - pixelSize * j + centrarEnElPlanoH - h, 1, 1);
                dir = planePoint - oLocal;

                DotDir dirMundo = localAMundo*dir;

                dirMundo = normalization(dirMundo);
                Ray rayoMundo( oMundo, dirMundo);

                DotDir inters, minInters;
                float minT = INFINITY, newT = INFINITY;
                for(int i = 0; i < objetos.size(); i++){
                    // Si no intersecta no se modifica newT
                    if(objetos[i]->instersects(rayoMundo, newT, inters)){
                        if(newT < minT){
                            minT = newT;
                            minTObject = i;
                            minInters = inters;
                        }
                    }
                }
                
                if(minTObject >= 0){
                    color[z] = objetos[minTObject]->getEmission();
                    minTObject = -1;
                }else{
                    color[z] = rgb(0,0,0);
                } 
            }  

            // Media del antialiasing
            float red = 0.0, green = 0.0, blue = 0.0;
            for(int z = 0; z < antiAliasing; ++z){
                red += color[z].r;
                green += color[z].g;
                blue += color[z].b;
            }

            red /= (float) antiAliasing;
            green /= (float) antiAliasing;
            blue /= (float) antiAliasing;
            newImagen.setRGB(i + j * planeW, rgb(red, green, blue));
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
        void lanzarRayos(vector<Figure*> objetos,  Image& imagen, int antiAliasing, const int nThreads = 8){
            
            if(planeH <= planeW){
                pixelSize = 2 / planeH;
            }else{
                pixelSize = 2 / planeW;
            }
            
            centrarEnElPlanoW = pixelSize * planeW / 2;
            centrarEnElPlanoH = pixelSize * planeH / 2;
            Image newImagen("", "render", planeW, planeH, 255, 1);
            std::vector<std::thread> th(nThreads);

            auto start = chrono::steady_clock::now();

            for(int i = 1; i < nThreads + 1; ++i){
                if(i!=nThreads){
                    th[i-1] = thread(&lanzarRayosParalelizado, std::ref(newImagen), 0, planeW, (i-1)*(planeH/nThreads),  i*(planeH/nThreads), antiAliasing, objetos, pixelSize, centrarEnElPlanoW, centrarEnElPlanoH, planeW, oLocal, oMundo, localAMundo, i-1);
                } else {
                    th[i-1] = thread(&lanzarRayosParalelizado, std::ref(newImagen), 0, planeW, (i-1)*(planeH/nThreads),  i*(planeH/nThreads), antiAliasing, objetos, pixelSize, centrarEnElPlanoW, centrarEnElPlanoH, planeW, oLocal, oMundo, localAMundo, i-1);
                }
            }
            
            for(int i = 1; i < nThreads + 1; ++i){
                th[i-1].join();
            }

            auto end = chrono::steady_clock::now();
            cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;

            imagen = newImagen;
        }
};

#endif