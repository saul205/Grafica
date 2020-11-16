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
const float airRefraction = 1.000293;

void lanzarRayosParalelizado(Image& newImagen, ConcurrentBoundedQueue& cbq, int antiAliasing, const BoundingVolume &scene, 
                        const float pixelSizeX, const float pixelSizeY, float centrarEnElPlanoW, float centrarEnElPlanoH, float planeW, Transformation localAMundo){

    std::uniform_real_distribution<float> distX(0.0, pixelSizeX);
    std::default_random_engine genX;
    auto randomX = std::bind(distX, genX);

    std::uniform_real_distribution<float> distY(0.0, pixelSizeY);
    std::default_random_engine genY;
    auto randomY = std::bind(distY, genY);

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
                    float h = randomX();
                    float w = randomY();

                    // Como f = 1 la tercera componente es fija
                    planePoint.setDotDir(- (pixelSizeX * i - centrarEnElPlanoW + w), - pixelSizeY * j + centrarEnElPlanoH - h, 1, 1);

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
                    bool intersecta = true;
                    while(intersecta) {
                        
                        intersecta = scene.intersect(rayoMundoRebotes, minTObject, interseccion, intersecciones);

                        if(intersecta && minTObject->hasEmission()) {
                            emisionFinalRayo = minTObject->getEmission(interseccion);
                            intersecta = false;
                        } else if(intersecta){
                            
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

                            // La normal es base[2]
                            float p = roussianRoulette();
                            //cout << p<< "   " << pk << "   " << ps << endl;
                            if(p < pk){ //Difuso

                                float i = roussianRoulette();
                                float azimuth = i * 2 * pi;

                                i = roussianRoulette();
                                float inclination = acos(sqrt(1 - i));
                                wi = DotDir(sin(inclination)*cos(azimuth), sin(inclination)*sin(azimuth), cos(inclination), 0);
                                
                                if(dotProduct(base[2], rayoMundoRebotes.getDir()) > 0){
                                    base[2] = DotDir(-base[2].getX(), -base[2].getY(), -base[2].getZ(), 0);
                                }
                                localAMundo.changeBase(base[0], base[1], base[2], interseccion);
                                mundoALocal = inverse(localAMundo);
                                
                                float coseno = abs(dotProduct(mundoALocal * base[2], wi));

                                
                                //cout << (minTObject->getDifRgb()*(coseno/(pi*pk))).r << endl;
                                emisionAcumulada = emisionAcumulada*(minTObject->getDifRgb()*(coseno/(pi*pk)));
                                //cout << "nuevo rayo " << newRay.getOrigen().toString() << " | " << newRay.getDir().toString() << endl;
 
                            }else if(p < pk + ps){ // Specular

                                //Inversión de las normales
                                if(dotProduct(base[2], rayoMundoRebotes.getDir()) > 0){
                                    base[2] = DotDir(-base[2].getX(), -base[2].getY(), -base[2].getZ(), 0);
                                }
                                localAMundo.changeBase(base[0], base[1], base[2], interseccion);
                                mundoALocal = inverse(localAMundo);

                                //Direccion reflejada
                                wi = getSpecularRay(mundoALocal*base[2], mundoALocal * rayoMundoRebotes.getDir());

                                //Producto de la BDRF
                                emisionAcumulada = emisionAcumulada*(minTObject->getSpecRgb() / ps);

                            }else if(p < pk + ps + pt){ // Refraccion

                                //Comprobación si estás dentro o fuera para calculo de indice refracción
                                float refractionIndx = minTObject->refractionIndex / airRefraction;
                                if(dotProduct(base[2], rayoMundoRebotes.getDir()) > 0){
                                    refractionIndx = airRefraction / minTObject->refractionIndex;
                                }

                                //Calculo de matrices
                                localAMundo.changeBase(base[0], base[1], base[2], interseccion);
                                mundoALocal = inverse(localAMundo);
                                // Snell's Law

                                DotDir normal = mundoALocal * base[2];
                                DotDir localRay = mundoALocal * rayoMundoRebotes.getDir();


                                float a = 1 - dotProduct(normal, localRay) * dotProduct(normal, localRay);

                                wi = sqrt(1.0f - refractionIndx * refractionIndx * a) * normal + refractionIndx * (localRay - dotProduct(normal, localRay) * normal);
                                emisionAcumulada = emisionAcumulada*(minTObject->getRefRgb() / pt);

                            }else{ // Absorcion
                                intersecta = false;
                            } 
                            wi = localAMundo * wi;
                            Ray newRay(interseccion, normalization(wi));
                            rayoMundoRebotes = newRay;
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
        void lanzarRayos(const BoundingVolume &scene,  Image& imagen, int antiAliasing, const int nThreads = 1){
            
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
                th[i] = thread(&lanzarRayosParalelizado, std::ref(imagen), std::ref(cbq), antiAliasing, scene, pixelSizeX, pixelSizeY, centrarEnElPlanoW, centrarEnElPlanoH, planeW, localAMundo);
            }
            
            for(int i = 0; i < nThreads; ++i){
                th[i].join();
            }

            auto end = chrono::steady_clock::now();
            cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;

        }
};

#endif