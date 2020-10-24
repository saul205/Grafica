#ifndef SENSOR_H
#define SENSOR_H

#include "DotDir.h"
#include "Transformation.h"
#include "Plane.h"
#include "Ray.h"
#include "Image.h"

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

        bool lanzarRayos(vector<shared_ptr<Figure>> objetos, Image& imagen){
            
            float pixelSize;
            if(planeH <= planeW){
                pixelSize = 2 / planeH;
            }else{
                pixelSize = 2 / planeW;
            }

            cout << pixelSize << endl;

            Image newImagen("algo", "render", planeW, planeH, 255, 1);

            shared_ptr<Figure> minTObject = nullptr;
            DotDir planePoint;
            DotDir dir;

            for(int i = 0; i < planeW; ++i){
                for(int j = 0; j < planeH; ++j){
                    // De momento tiramos el rayo a una esquina del píxel
                    // Origen en local es 0,0,0,1
                    // Como f = 1 la tercera componente es fija
                    planePoint.setDotDir(pixelSize * i - pixelSize * planeW / 2, - pixelSize * j + pixelSize * planeH / 2, 1, 1);
                    dir = planePoint - oLocal;
                    normalization(dir);

                    Ray rayoLocal( oLocal, dir);

                    //cout << dir.toString() << endl;

                    DotDir dirMundo = localAMundo*dir;

                    Ray rayoMundo( oMundo, dirMundo);

                    float minT = INFINITY, newT = INFINITY;
                    for(auto object : objetos){
                        
                        // Si no intersecta no se modifica newT
                        if(object->instersects(rayoMundo, newT)){

                            //cout << "Objeto  " << newT << endl; 
                            if(newT < minT){
                                minT = newT;
                                minTObject = object;
                            }
                        }
                    }

                    //cout << endl; 

                    //Mostrar en pantalla
                    if(minTObject != nullptr){
                        newImagen.setRGB(i + j * planeW, minTObject->getEmission());
                        minTObject = nullptr;
                    }else{
                        newImagen.setRGB(i + j * planeW, {255, 255, 255});
                    }

                    
                }
            }

            imagen = newImagen;

            return true;
        }

};

#endif