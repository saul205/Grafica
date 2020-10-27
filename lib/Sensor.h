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
#include <stdlib.h>
#include <thread>

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
        bool lanzarRayos(vector<shared_ptr<Figure>> objetos,  Image& imagen, int antiAliasing, const int nThreads = 8){
            
            if(planeH <= planeW){
                pixelSize = 2 / planeH;
            }else{
                pixelSize = 2 / planeW;
            }
            
            centrarEnElPlanoW = pixelSize * planeW / 2;
            centrarEnElPlanoH = pixelSize * planeH / 2;
            Image newImagen("", "render", planeW, planeH, 255, 1);
            srand(0);
            thread th[nThreads];

            for(int i = 1; i < nThreads + 1; ++i){
                if(i!=nThreads){
                    th[i-1] = thread(&lanzarRayosParalelizado, newImagen, (i-1)*(planeW/nThreads), i*(planeW/nThreads), 0, planeH, antiAliasing, objetos);
                } else {
                    th[i-1] = thread(&lanzarRayosParalelizado, newImagen, (i-1)*(planeW/nThreads), planeW, 0, planeH, antiAliasing, objetos);
                }
            }
            
            for(int i = 1; i < nThreads + 1; ++i){
                th[i-1].join();
            }

            imagen = newImagen;

            return true;
        }

};


void lanzarRayosParalelizado(Image& newImagen, int wminlimit, int wmaxlimit, int hminlimit,
            int hmaxlimit, int antiAliasing, vector<shared_ptr<Figure>> objetos){

    shared_ptr<Figure> minTObject = nullptr;
    DotDir planePoint, dir;

    for(int i = wminlimit; i < wmaxlimit; ++i){
        for(int j = hminlimit; j < hmaxlimit; ++j){

            rgb color[antiAliasing];
            for(int z = 0; z < antiAliasing; ++z){

                float h = (((float) rand()) / RAND_MAX) * pixelSize;
                float w = (((float) rand()) / RAND_MAX) * pixelSize;

                // De momento tiramos el rayo a una esquina del píxel
                // Origen en local es 0,0,0,1
                // Como f = 1 la tercera componente es fija
                planePoint.setDotDir(pixelSize * i - centrarEnElPlanoW + w, - pixelSize * j + centrarEnElPlanoH - h, 1, 1);
                dir = planePoint - oLocal;

                DotDir dirMundo = localAMundo*dir;

                dirMundo = normalization(dirMundo);
                Ray rayoMundo( oMundo, dirMundo);

                //cout << dirMundo.toString() << endl;

                DotDir inters, minInters;
                float minT = INFINITY, newT = INFINITY;
                for(auto object : objetos){
                    // Si no intersecta no se modifica newT
                    if(object->instersects(rayoMundo, newT, inters)){

                        if(newT < minT){
                            minT = newT;
                            minTObject = object;
                            minInters = inters;
                        }
                    }
                }
                
                //Mostrar en pantalla
                if(minTObject != nullptr){
                    color[z] = minTObject->getEmission();
                    minTObject = nullptr;
                }else{
                    color[z] = rgb(0,0,0);
                } 
            }  

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

#endif