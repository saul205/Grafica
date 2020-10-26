#ifndef SENSOR_H
#define SENSOR_H

#include "DotDir.h"
#include "Transformation.h"
#include "Plane.h"
#include "Ray.h"
#include "Image.h"
#include "LightSource.h"
#include <memory>
#include <stdlib.h>

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
            // No se está usando el prjection plane
            projectionPlane.setNormal(fMundo);
        }

        float renderEq(DotDir intersection, LightSource luz, shared_ptr<Figure> fig){

            DotDir shadowR = luz.getPosition() - intersection;
            float shMod = shadowR.mod();
            return luz.getIntensity() / (shMod * shMod);
        }

        bool lanzarRayos(vector<shared_ptr<Figure>> objetos, vector<LightSource> luces, Image& imagen, int antiAliasing){
            
            float pixelSize;
            if(planeH <= planeW){
                pixelSize = 2 / planeH;
            }else{
                pixelSize = 2 / planeW;
            }

            cout << pixelSize << endl;

            Image newImagen("", "render", planeW, planeH, 255, 1);

            shared_ptr<Figure> minTObject = nullptr;
            DotDir planePoint;
            DotDir dir;
            srand(0);

            const float centrarEnElPlanoW = pixelSize * planeW / 2;
            const float centrarEnElPlanoH = pixelSize * planeH / 2;
 
            for(int i = 0; i < planeW; ++i){
                for(int j = 0; j < planeH; ++j){

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
/*
                        if(i == 100 && j == 800){
                            cout << minInters.toString() << endl; 
                        }
*/
                        //Mostrar en pantalla
                        if(minTObject != nullptr){

                            float lightPower = 0;

                            for(auto luz : luces){

                                Ray shRay(minInters, normalization(luz.getPosition() - inters));

                                bool iluminado = true;
                                for(int o = 0; o < objetos.size() && iluminado; o++){
                                // Si no intersecta no se modifica newT
                                    if(objetos[o]->instersects(shRay, minT, minInters)){

                                        if(minT < (luz.getPosition() - minInters).mod() && minT > 0){
                                            iluminado = false;
                                        }
/*
                                        if(i == 100 && j == 800 && minT == 0){
                                            cout << "Intersecta con el objeto " << o << endl; 
                                        }
                                        */
                                    }
                                }

                                if(iluminado){
                                    //lightPower += renderEq(minInters, luz, minTObject);
                                    lightPower += luz.getIntensity();
                                    if(lightPower > 1) lightPower = 1;
                                }
                            }


                            color[z] = minTObject->getEmission();
                            color[z].r *= lightPower;
                            color[z].g *= lightPower;
                            color[z].b *= lightPower;

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

            imagen = newImagen;

            return true;
        }

};

#endif