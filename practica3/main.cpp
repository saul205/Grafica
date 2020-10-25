#include <iostream>
#include "../lib/Sensor.h"
#include "../lib/Sphere.h"
#include <memory>

int main(){

    float W = 1000, H = 500;
    Image newImage;

    DotDir camera[4];
    camera[0].setDotDir(1, 0, 0, 0);
    camera[1].setDotDir(0, 1, 0, 0);
    camera[2].setDotDir(0, 0, 1, 0);
    camera[3].setDotDir(0, 0, -10, 1);

    Sensor renderer(camera[0], camera[1], camera[2], camera[3], W, H);

    vector<shared_ptr<Figure>> figuras;

    DotDir normal(1, 0, -1, 0);
    shared_ptr<Figure> plano(new Plane(normal, 3));
    plano->setRgb(rgb(255,0,0));

    DotDir normal2(-1, 0, -1, 0);
    shared_ptr<Figure> plano2(new Plane(normal2, 3));
    plano2->setRgb(rgb(0,255,0));

    DotDir normal3(0, 1, -1, 0);
    shared_ptr<Figure> plano3(new Plane(normal3, 3));
    plano3->setRgb(rgb(0,0,255));

    DotDir center(0,0,-3,0);
    DotDir axis(0,8,0,0);
    DotDir city(0,0,-7,0);
    if(checkRadius(axis, center, city) ){
        shared_ptr<Figure> esfera(new Sphere(center, axis, city));
        esfera->setRgb(rgb(255,255,0));
        figuras.push_back(esfera);
    } else {
        cout << "Error en la esfera." << endl;
    }

    figuras.push_back(plano);
    figuras.push_back(plano2);
    figuras.push_back(plano3);

    bool success = renderer.lanzarRayos(figuras, newImage);
    if(success){
        cout << "Escribo" << endl;
        escribirbmp("render.bmp", newImage, 255);
    }
    return 0;
}