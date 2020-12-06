#ifndef RAY_H
#define RAY_H

#include "DotDir.h"

const float airRefraction = 1.000293;

class Ray{
    private:

        DotDir origen;
        DotDir direction;

    public:

        Ray(DotDir _origen, DotDir direccion){
            origen = _origen;
            direction = direccion;
        }

        DotDir getDir(){
            return direction;
        }

        DotDir getOrigen(){
            return origen;
        }
};

// sin^2(tita2) = (n1/n2)^2*(1-cos²(titai))
float getSin2T(float costitaI, float roiEntrante, float roiSaliente){
    float cociente = (roiEntrante/roiSaliente);
    return cociente*cociente*(1.f-costitaI*costitaI);
}

// https://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
// Calcula la proporción entre refracción y reflexión usando las leyes de Fresnel
float fresnel(const DotDir& incidente, const DotDir& normal, const float& roiEntrante){
    
    // Coseno incidente
    float cosenoI = dotProduct(incidente, normal);

    float entrante = airRefraction;
    float saliente = roiEntrante;
    
    // Si mayor que 0, saliendo de la esfera, se intercambian los roi
    if (cosenoI < 0) { 
        cosenoI = -cosenoI; 
    } 
    else if(cosenoI > 0){ 
        entrante = roiEntrante;
        saliente = airRefraction;
    }

    float sin2T = getSin2T(cosenoI, entrante, saliente);

    // Reflexión total
    if(sin2T > 1.0f){
        return 1.0f;
    }else{
        float cosenoT = sqrtf(std::max(0.f, 1.f - sin2T));
        float r1root = (entrante*cosenoI - saliente*cosenoT) / (entrante*cosenoI + saliente*cosenoT);
        float r1 = r1root*r1root;
        float r2root = (saliente*cosenoI - entrante*cosenoT) / (saliente*cosenoI + entrante*cosenoT);
        float r2 = r2root*r2root;
        return (r1 + r2) / 2.0f;
    }
}

// Obtiene el rayo especular a partir de la normal y el incidente
// http://paulbourke.net/geometry/reflected/
DotDir getSpecularRay(DotDir normal, DotDir wo){
    //cout << "Normal: " << normal.toString() << endl;
    //cout << "Wo: " << wo.toString() << endl;
    DotDir dd = wo - 2.0f*(dotProduct(normal,wo))*normal;
    return dd;
}

// Obtiene un rayo a partir del muestreo del coseno
DotDir getCosineSamplingRay(float r1, float r2){
    float azimuth = r1 * 2 * 3.141592f;
    float inclination = acos(sqrt(r2));
    return DotDir(sin(inclination)*cos(azimuth), sin(inclination)*sin(azimuth), cos(inclination), 0);                           
}

// Obtiene un rayo refractado
// https://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
DotDir getRefractedRay(DotDir incidente, DotDir normal, float refractionIndex){

    // Si coseno < 0 el rayo es incidente
    // En caso contrario, forma ángulo < 90º con la normal, sale del objeto
    float entrante = airRefraction;
    float saliente = refractionIndex; 

    // Coseno rayo incidente con la normal
    float cosenoI = dotProduct(incidente, normal);

    // Entrando a la esfera, ángulo mayor que 90º, el coseno realmente, ajusto coseno incidente
    // para que se corresponda con el formado por el rayo en sentido contrario (acorde a diapos)
    if (cosenoI < 0) { 
        cosenoI = -cosenoI; 
    } else {
        normal = -normal;
        saliente = airRefraction;
        entrante = refractionIndex;
    }

    // n0 / n1
    float es = entrante /saliente;
    return es*incidente + (es * cosenoI - sqrtf(1.0f - getSin2T(cosenoI, entrante, saliente))) * normal;
}

#endif