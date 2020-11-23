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

const int sizeCuadrante = 5;
const float fov = 1;
const float airRefraction = 1.000293;

float fresnel(const DotDir& incidente, const DotDir& normal, const float& refractionIndex){
    float cosi = dotProduct(incidente, normal);
    if(cosi < -1) cosi = -1;
    if(cosi > 1) cosi = 1;

    float etai = 1, etat = refractionIndex;
    if(cosi > 0){ std::swap(etai, etat); }

    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));

    if(sint >= 1.0f){
        return 1.0f;
    }else{
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = (etat * cosi - etai * cost) / (etat * cosi + etai * cost);
        float Rp = (etai * cosi - etat * cost) / (etai * cosi + etat * cost);
        return (Rs * Rs + Rp * Rp) / 2;
    }
}

void getMaterialProbabilities(shared_ptr<Figure> f, Ray rayo, DotDir normal, float& pk, float& ps, float& pt, bool bounce){

    if (f->material.isDielectrico()){
        float kr = fresnel(rayo.getDir(), normal, f->refractionIndex);
        //cout << "KR " << kr << endl;
        ps = kr;
        pt = 1.0f - ps;
        pk = 0;

        f->material.ks = rgb(kr, kr, kr);
        f->material.kt = rgb(1 - kr, 1 - kr, 1 - kr);
    }
    else{
        pk = f->material.kd.maximo();
        ps = f->material.ks.maximo();
        pt = f->material.kt.maximo();
    }

    float maxes = pk + ps + pt;
    pk = bounce ? 0.9 * (pk / maxes) : (pk / maxes);
    ps = bounce ? 0.9 * (ps / maxes) : (ps / maxes);
    pt = bounce ? 0.9 * (pt / maxes) : (pt / maxes);
}


// Obtiene un rayo a partir del muestreo del coseno
DotDir getCosineSamplingRay(float r1, float r2){
    float azimuth = r1 * 2 * pi;
    float inclination = acos(sqrt(r2));
    return DotDir(sin(inclination)*cos(azimuth), sin(inclination)*sin(azimuth), cos(inclination), 0);                           
}

// Obtiene el rayo especular a partir de la normal y el incidente
DotDir getSpecularRay(DotDir normal, DotDir wo){
    return wo - 2.0f*(dotProduct(wo,normal))*normal;
}

// Obtiene un rayo refractado
DotDir getRefractedRay(DotDir incidente, DotDir normal, float refractionIndex){
    float cosi = dotProduct(incidente, normal);
    if(cosi < -1) cosi = -1;
    if(cosi > 1) cosi = 1;

    float etai = 1, etat = refractionIndex; 
    DotDir n = normal; 
    if (cosi < 0) { cosi = -cosi; }
    else { 
        std::swap(etai, etat); 
        n = DotDir(-normal.getX(), -normal.getY(), -normal.getZ(), 1); 
    } 
    float eta = etai / etat; 
    float k = 1 - eta * eta * (1 - cosi * cosi); 
    return k < 0 ? DotDir(0,0,0,1) : eta * incidente + (eta * cosi - sqrtf(k)) * n;
}

void lanzarRayosParalelizado(Image& newImagen, ConcurrentBoundedQueue& cbq, int antiAliasing, const BoundingVolume &scene, vector<LightSource> luces,
                        const float pixelSizeX, const float pixelSizeY, float centrarEnElPlanoW, float centrarEnElPlanoH, float planeW, Transformation localAMundo){

    std::uniform_int_distribution<int> dist(0, luces.size()-1);
    std::default_random_engine gen;
    auto lucesRoussianRoulette = std::bind(dist, gen);

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
                    float h = roussianRoulette()*pixelSizeX;
                    float w = roussianRoulette()*pixelSizeY;

                    // Como f = 1 la tercera componente es fija
                    planePoint.setDotDir(- (pixelSizeX * i - centrarEnElPlanoW + w), - pixelSizeY * j + centrarEnElPlanoH - h, 1, 1);

                    DotDir oLocal = DotDir(0,0,0,1);
                    Ray rayoMundo( localAMundo*oLocal, normalization(localAMundo*(planePoint-oLocal)));
                  
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
                            getMaterialProbabilities(minTObject, rayoMundoRebotes, base[1], pk, ps, pt, bounce);

                            Transformation mundoALocal, localToMundo;

                            if(p < pk){                 //Difuso
 
                                wi = getCosineSamplingRay(roussianRoulette(), roussianRoulette());

                                if(dotProduct(base[2], rayoMundoRebotes.getDir()) > 0)
                                    base[2] = DotDir(-base[2].getX(), -base[2].getY(), -base[2].getZ(), 0);
                                localToMundo.changeBase(base[0], base[1], base[2], interseccion);
                                mundoALocal = inverse(localToMundo);
                                
                                float coseno = abs(dotProduct(mundoALocal * base[2], wi));
                                emisionAcumulada = emisionAcumulada*(minTObject->getDifRgb()*(coseno/(pi*pk)));
 
                            }else if(p < pk + ps){      // Especular

                                if(dotProduct(base[2], rayoMundoRebotes.getDir()) > 0)
                                    base[2] = DotDir(-base[2].getX(), -base[2].getY(), -base[2].getZ(), 0);
                                localToMundo.changeBase(base[0], base[1], base[2], interseccion);
                                mundoALocal = inverse(localToMundo);

                                //Direccion reflejada
                                wi = getSpecularRay(mundoALocal*base[2], mundoALocal * rayoMundoRebotes.getDir());
                                emisionAcumulada = emisionAcumulada*(minTObject->getSpecRgb() / ps);

                            }else if(p < pk + ps + pt){ // Refraccion

                                //Calculo de matrices
                                localToMundo.changeBase(base[0], base[1], base[2], interseccion);
                                mundoALocal = inverse(localToMundo);

                                // Inversión de las normales ya incluida
                                wi = getRefractedRay(mundoALocal*rayoMundoRebotes.getDir(), mundoALocal*base[2], minTObject->refractionIndex);
                                emisionAcumulada = emisionAcumulada*(minTObject->getRefRgb() / pt);
                            }
                            else{ // Absorcion
                                intersecta = false;
                            }

                            wi = localToMundo * wi;
                            Ray newRay(interseccion, normalization(wi));
                            rayoMundoRebotes = newRay;
                            
                            if(luces.size() > 0){

                                int light = lucesRoussianRoulette();
                                DotDir shadowRay = luces[light].getPosition() - interseccion;
                                Ray newShadowRay(interseccion, normalization(shadowRay));
                                DotDir punto = interseccion;

                                bool luzIntersecta = scene.intersect(newShadowRay, minTObject, interseccion, intersecciones);

                                if(!luzIntersecta || (interseccion - punto).mod() > shadowRay.mod())
                                    emisionFinalRayo = emisionFinalRayo + emisionAcumulada*(luces[light].getEmission() / (shadowRay.mod() * shadowRay.mod()));
                            }
                        }
                        bounce = true;
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
        void lanzarRayos(const BoundingVolume &scene, vector<LightSource> luces, Image& imagen, int antiAliasing, const int nThreads = 1){
            
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