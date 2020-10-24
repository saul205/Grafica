#include <iostream>
#include "../lib/Sensor.h"
#include "../lib/Sphere.h"

void main(){

    float W = 1000, H = 500;
    Image newImage;

    DotDir camera[4];
    camera[0].setDotDir(1, 0, 0, 0);
    camera[1].setDotDir(0, 1, 0, 0);
    camera[2].setDotDir(0, 0, 1, 0);
    camera[3].setDotDir(0, 0, -10, 1);

    Sensor renderer(camera[0], camera[1], camera[2], camera[3], W, H);

    vector<shared_ptr<Figure>> figuras;

    /*DotDir sphere[3];
    sphere[0].setDotDir(1, 0, 0, 0);
    sphere[1].setDotDir(0, 2, 0, 0);
    sphere[2].setDotDir(0, 0, 1, 0);
    shared_ptr<Figure> esfera(new Sphere(sphere[0], sphere[1], sphere[2]));
    figuras.push_back(esfera);*/

    DotDir normal(1, 0, -1, 0);
    shared_ptr<Figure> plano(new Plane(normal, 0));
    plano->setRgb({255, 0, 0});

    DotDir normal2(-1, 0, -1, 0);
    shared_ptr<Figure> plano2(new Plane(normal2, 0));
    plano2->setRgb({0, 255, 0});

    DotDir normal3(0, 1, -1, 0);
    shared_ptr<Figure> plano3(new Plane(normal3, 0));
    plano3->setRgb({0, 0, 255});

    figuras.push_back(plano);
    figuras.push_back(plano2);
    figuras.push_back(plano3);

    bool success = renderer.lanzarRayos(figuras, newImage);
    if(success){
        cout << "Escribo" << endl;
        escribirbmp("render.bmp", newImage, 255);
    }
}