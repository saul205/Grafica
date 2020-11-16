#ifndef SENSOR_H
#define SENSOR_H

#include "ConcurrentBoundedQueue.h"
#include "FigureProperties.h"
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

void lanzarRayosParalelizado(Image& newImagen, ConcurrentBoundedQueue& cbq, int antiAliasing, const BoundingVolume &scene, 
                        const float pixelSize, float centrarEnElPlanoW, float centrarEnElPlanoH, float planeW, Transformation localAMundo){

    std::uniform_real_distribution<float> dist(0.0, pixelSize);
    std::default_random_engine gen;
    auto random = std::bind(dist, gen);

    std::uniform_real_distribution<float> dist2(0.0, 1);
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
                    float h = random();
                    float w = random();

                    // Como f = 1 la tercera componente es fija
                    planePoint.setDotDir(- (pixelSize * i - centrarEnElPlanoW + w), - pixelSize * j + centrarEnElPlanoH - h, 1, 1);

                    DotDir oLocal = DotDir(0,0,0,1);

                    DotDir dirMundo = localAMundo*(planePoint-oLocal);

                    dirMundo = normalization(dirMundo);
                    Ray rayoMundo( localAMundo * oLocal, dirMundo);
                  
                    std::shared_ptr<Figure> minTObject;
                    DotDir interseccion;

                    //--------Path Tracing---------
                    rgb emisionAcumulada(1, 1, 1);
                    rgb emisionFinalRayo(0, 0, 0);
                    Ray rayoMundoRebotes = rayoMundo;
                    bool bounce = false;
                    bool yesyes = false;
                    bool intersecta = true;
                    float distance = 0;
                    while(intersecta) {
                        
                        intersecta = scene.intersect(rayoMundoRebotes, minTObject, interseccion, intersecciones);
                        //if(bounce) cout << "tr" << endl;
                        //if(yesyes) {cout << "true" << endl;}
                        if(intersecta && minTObject->hasEmission()) {
                            distance += (interseccion - rayoMundoRebotes.getOrigen()).mod();
                            emisionFinalRayo = minTObject->getEmission(interseccion) / (distance * distance);
                            //cout << "emisionFinalRayo:   " << emisionFinalRayo.r << "  "  << emisionFinalRayo.g << "   " << emisionFinalRayo.b << "   "<< endl;
                            //cout << "emisionAcumulada:   " << emisionAcumulada.r << "  "  << emisionAcumulada.g << "   " << emisionAcumulada.b << "   "<< endl;
                            //if(bounce) {cout << "jj" << endl;};
                            intersecta = false;
                        } else if(intersecta){

                            distance += (interseccion - rayoMundoRebotes.getOrigen()).mod();
                            
                            //Ruleta Rusa
                            float pk = minTObject->material.kd.maximo();
                            float ps = minTObject->material.ks.maximo();
                            float pt = minTObject->material.kt.maximo();
                            float maxes = pk + ps + pt;

                            pk = bounce ? 0.9 * (pk / maxes) : (pk / maxes);
                            ps = bounce ? 0.9 * (ps / maxes) : (ps / maxes);
                            pt = bounce ? 0.9 * (pt / maxes) : (pt / maxes);

                            DotDir base[3], wi;
                            //cout << "Antes: " <<  interseccion.toString() << endl;
                            minTObject->getBase(interseccion, base[0], base[1], base[2]);
                            //cout << "Después :" << base[2].toString() << endl;
                            Transformation mundoALocal, localAMundo;
                            // La normal es base[1]
                            localAMundo.changeBase(base[0], base[1], base[2], interseccion);
                            mundoALocal = inverse(localAMundo);
                            float p = roussianRoulette();
                            //cout << p<< "   " << pk << "   " << ps << endl;
                            if(p < pk){ //Difuso

                                float i = roussianRoulette();
                                float azimuth = i * 2 * pi;

                                i = roussianRoulette();
                                float inclination = acos(sqrt(1 - i));
                                wi = DotDir(sin(inclination)*cos(azimuth), sin(inclination)*sin(azimuth), cos(inclination), 0);
                                
                                DotDir normal = mundoALocal*base[2];

                                if(dotProduct(normal, mundoALocal * rayoMundoRebotes.getDir()) > 1){
                                    cout << "Escalar" << dotProduct(normal, mundoALocal * rayoMundoRebotes.getDir());
                                    normal = DotDir(-normal.getX(), -normal.getY(), -normal.getZ(), 0);
                                }
                                
                                
                                float coseno = abs(dotProduct(normal, wi));

                                wi = localAMundo * wi;
                                //cout << (minTObject->getDifRgb()*(coseno/(pi*pk))).r << endl;
                                emisionAcumulada = emisionAcumulada*(minTObject->getDifRgb()*(coseno/(pi*pk)));
                                Ray newRay(interseccion, wi);
                                rayoMundoRebotes = newRay;

                                //cout << "nuevo rayo " << newRay.getOrigen().toString() << " | " << newRay.getDir().toString() << endl;
 
                            }else if(p < pk + ps){ // Specular
                                //if(bounce) cout << "Rebota" << endl;
                                // wr = wi
                                //cout << " From: " << rayoMundoRebotes.getOrigen().getX() <<"   " << rayoMundoRebotes.getOrigen().getY() << "  " << rayoMundoRebotes.getOrigen().getZ() << endl;
                                //cout << "Incide: " << rayoMundoRebotes.getDir().getX() <<"   " << rayoMundoRebotes.getDir().getY() << "  " << rayoMundoRebotes.getDir().getZ() << endl;
                                //cout << "Interseccion: " << interseccion.getX() <<"   " << interseccion.getY() << "  " << interseccion.getZ() << endl;
                                //DotDir wo = mundoALocal * rayoMundoRebotes.getDir();
                                //Reflexión perfecta
                                //cout << "base: " << base[2].toString() << endl;
                                //cout << "LocalAnetes: " << wo.getX() <<"   " << wo.getY() << "  " << wo.getZ() << endl;
                                wi = getSpecularRay(mundoALocal*base[2], mundoALocal * rayoMundoRebotes.getDir());
                                //cout << "LocalResul: " << wi.getX() <<"   " << wi.getY() << "  " << wi.getZ() << endl;
                                wi = localAMundo * wi;
                                emisionAcumulada = emisionAcumulada*(minTObject->getSpecRgb() / ps);

                                //cout << minTObject->getSpecRgb().r << " - " << minTObject->getSpecRgb().g << " - " << minTObject->getSpecRgb().b << " - " << ps;
                                // cout << "emisionAcumulada:   " << emisionAcumulada.r << "  "  << emisionAcumulada.g << "   " << emisionAcumulada.b << "   "<< endl;
                                Ray newRay(interseccion, wi);
                                rayoMundoRebotes = newRay;
                                //cout << "Resul: " << wi.getX() <<"   " << wi.getY() << "  " << wi.getZ() << endl;

                            }else if(p < pk + ps + pt){ // Refraccion
                                // Snell's Law
                                float inclination = angle(mundoALocal * rayoMundoRebotes.getDir(), mundoALocal * base[2]);
                                float azimuth = angle(mundoALocal * rayoMundoRebotes.getDir(), mundoALocal * base[0]);

                                FigureProperties f;

                                inclination = 180 + f.getSnellRefractionAngle(inclination);
                                azimuth = 180 + f.getSnellRefractionAngle(azimuth);

                                wi = DotDir(sin(inclination)*cos(azimuth), sin(inclination)*sin(azimuth), cos(inclination), 0);
                                
                                DotDir normal = mundoALocal*base[2];
                                
                                float coseno = abs(dotProduct(mundoALocal*base[2], wi));

                                wi = localAMundo * wi;
                                //cout << (minTObject->getDifRgb()*(coseno/(pi*pk))).r << endl;
                                emisionAcumulada = emisionAcumulada*(minTObject->getRefRgb()*(coseno/(pi*pt)));
                                Ray newRay(interseccion, wi);
                                rayoMundoRebotes = newRay;

                            }else{ // Absorcion
                                intersecta = false;
                            } 
                            //Lanzar nuevo rayo

                            //Calcular BRDF
                            //luz *= fr(x, wi, wo) * |n * wi| / p(wi)
                            //Parar si no intersecta, absorbe -> luz del camino = 0
                            //Si objeto emisor -> luz con valor
                            /*bool il = scene.intersect(rayoMundoRebotes, minTObject, interseccion, intersecciones);
                            bounce = true;
                            if(il){ 
                                //cout << "Rebote explota." << endl; cout << "bounce: " << intersecta << endl;
                                if(minTObject->hasEmission()){
                                    yesyes= true;
                                    cout << "lol" << endl;
                                    intersecta = true;
                                    cout << "intersecta: " << intersecta << endl;
                                }
                            }*/
                        }
                        bounce = true;
                        //cout << "bounce: " << intersecta << endl;
                    }

                    //cout << emisionAcumulada.r<< "  " << emisionAcumulada.r << "  " << emisionAcumulada.b << endl;
                    //cout << emisionFinalRayo.r << "  " << emisionFinalRayo.g << "  " << emisionFinalRayo.b << endl;
                    emisionFinal = emisionFinal + emisionFinalRayo * emisionAcumulada;
                }  

                emisionFinal = emisionFinal / (float) antiAliasing;
                newImagen.setRGB(i + j * planeW, emisionFinal);
            }
        }
    }
    //cout << "Intersecciones: " << intersecciones << endl;
}

class Sensor{
    private:

        Transformation localAMundo;
        float planeW, planeH;
        float pixelSize, centrarEnElPlanoW, centrarEnElPlanoH;

    public:

        Sensor(){}

        Sensor(DotDir _r, DotDir _u, DotDir _f, DotDir _o, float planeWidth, float planeHeight) :
                planeW(planeWidth), planeH(planeHeight) {

            localAMundo.changeBase(_r, _u, _f, _o);
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

            centrarEnElPlanoW = pixelSize * planeW / 2;
            centrarEnElPlanoH = pixelSize * planeH / 2;

            std::vector<std::thread> th(nThreads);
            auto start = chrono::steady_clock::now();

            for(int i = 0; i < nThreads; ++i){
                th[i] = thread(&lanzarRayosParalelizado, std::ref(imagen), std::ref(cbq), antiAliasing, scene, pixelSize, centrarEnElPlanoW, centrarEnElPlanoH, planeW, localAMundo);
            }
            
            for(int i = 0; i < nThreads; ++i){
                th[i].join();
            }

            auto end = chrono::steady_clock::now();
            cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;

        }
};

#endif