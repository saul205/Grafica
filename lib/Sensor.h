#ifndef SENSOR_H
#define SENSOR_H

#include "DotDir.h"
#include "Transformation.h"
#include "Plane.h"
#include "Ray.h"

class Sensor{
    private:

        DotDir lMundo, uMundo, fMundo, oMundo;
        DotDir lLocal, uLocal, fLocal, oLocal;
        Transformation localAMundo;
        float planeW, planeH;
        Plane projectionPlane;

    public:

        Sensor(DotDir _l, DotDir _u, DotDir _f, DotDir _o, float planeWidth, float planeHeight) :
                lMundo(_l), uMundo(_u), fMundo(_f), oMundo(_o), planeW(planeWidth), planeH(planeHeight) {

            lLocal = DotDir(1, 0, 0, 0);
            uLocal = DotDir(0, 1, 0, 0);
            fLocal = DotDir(0, 0, 1, 0);
            oLocal = DotDir(0, 0, 0, 1);

            localAMundo.changeBase(lMundo, uMundo, fMundo, oMundo);
            projectionPlane.setNormal(fMundo);
        }

        void lanzarRayos(){

            const float pixelW = 1 / planeW;
            const float pixelH = 1 / planeH;

            for(int i = 0; i < planeH; ++i){
                for(int j = 0; j < planeW; ++j){

                    // De momento tiramos el rayo a una esquina del píxel
                    // Origen en local es 0,0,0,1
                    // Como f = 1 la tercera componente es fija
                    DotDir dir( pixelW*j, planeH*i, 1, 0);
                    normalization(dir);

                    Ray rayoLocal( oLocal, dir);

                    DotDir dirMundo = localAMundo*dir;
                    Ray rayoMundo( oMundo, dirMundo);

                    float minT = 0.0, newT;
                    colores minColores, newColores;
                    for(todas las figura){

                        // Si no intersecta no se modifica newT
                        if(figura.instersects(rayoMundo, newT, newColores)){
                            if(newT < minT){
                                minT = newT;
                                minColores = newColores;
                            }
                        }
                    }
                }
            }

        }

};

#endif