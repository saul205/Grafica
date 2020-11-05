#include <iostream>
#include "../lib/Sensor.h"
#include "../lib/Sphere.h"
#include "../lib/ToneMapper.h"
#include <memory>

int main(){

    float W = 1600, H = 900;
    Image newImage("P3", "IMG", 1600, 900, 255, 255);

    DotDir camera[4];
    camera[0].setDotDir(1, 0, 0, 0);
    camera[1].setDotDir(0, 1, 0, 0);
    camera[2].setDotDir(0, 0, 1, 0);
    camera[3].setDotDir(0, 0, -10, 1);

    Sensor renderer(camera[0], camera[1], camera[2], camera[3], W, H);

    std::string file, file2;
    std::cout << "Introduce una imagen: " << std::endl;
    std::cin >> file;
    Image imagen = leer(file);
    ToneMapper tm;
    tm.Filmic(imagen);
    
    vector<Figure*> figuras;

    /*
        =================================================
        ESCENA DE LA CARA
        =============================================

            DotDir normal(6, 0, -1, 0);
    Figure* plano = new Plane(normal, 65);
    plano->setRgb(rgb(255,0,0));

    DotDir normal2(-6, 0, -1, 0);
    Figure* plano2 = new Plane(normal2, 65);
    plano2->setRgb(rgb(0,255,0));

    DotDir normal3(0, 1, 0, 0);
    Figure* plano3 = new Plane(normal3, 3);
    plano3->setRgb(rgb(0,0,255));

    DotDir normal4(0, 0, -1, 0);
    Figure* plano4 = new Plane(normal4, 0);
    plano4->setRgb(rgb(124,50,255));

    DotDir center(0,0,-3,1);
    DotDir axis(0,8,0,0);
    DotDir city(0,0,-7,1);
    if(checkRadius(axis, center, city) ){
        Figure* esfera = new Sphere(center, axis, city);
        esfera->setRgb(rgb(255,255,0));
        figuras.push_back(esfera);
    } else {
        cout << "Error en la esfera." << endl;
    }

    DotDir center2(0.6,0.4,-6,1);
    DotDir axis2(0,2,0,0);
    DotDir city2(0.6,0.4,-7,1);
    if(checkRadius(axis2, center2, city2) ){
        Figure* esfera2 = new Sphere(center2, axis2, city2);
        esfera2->setRgb(rgb(255,49,134));
        figuras.push_back(esfera2);
    } else {
        cout << "Error en la esfera2." << endl;
    }

    DotDir center3(-0.6,0.4,-6,1);
    DotDir axis3(0,2,0,0);
    DotDir city3(-0.6,0.4,-7,1);
    if(checkRadius(axis3, center3, city3) ){
        Figure* esfera3 = new Sphere(center3, axis3, city3);
        esfera3->setRgb(rgb(255,49,134));
        figuras.push_back(esfera3);
    } else {
        cout << "Error en la esfera3." << endl;
    }

    DotDir center4(0,-1,-6,1);
    DotDir axis4(0,2.5,0,0);
    DotDir city4(0,-1,-7.25,1);
    if(checkRadius(axis4, center4, city4) ){
        Figure* esfera4 = new Sphere(center4, axis4, city4);
        esfera4->setRgb(rgb(0,0,0));
        figuras.push_back(esfera4);
    } else {
        cout << "Error en la esfera4." << endl;
    }

    figuras.push_back(plano);
    figuras.push_back(plano2);
    figuras.push_back(plano3);
    figuras.push_back(plano4);

    // Pusheo un triángulo, las cejas
    Figure* triangle = new Triangle(DotDir(-0.8,0.9,-8,1), DotDir(-0.7,1.2,-8,1), DotDir(-0.1,0.5,-8,1));
    Figure* triangle2 = new Triangle(DotDir(0.1,0.5,-8,1), DotDir(0.7,1.2,-8,1), DotDir(0.8,0.9,-8,1));
    triangle->setRgb(rgb(255,255,255));
    triangle2->setRgb(rgb(255,255,255));
    figuras.push_back(triangle);
    figuras.push_back(triangle2);

    */

    /*
    
        ================================================
        PLANOS TRIÁNGULOS
        ===============================================
    
    Figure* triangle = new Triangle(DotDir(-3840,2140,2000,1), DotDir(0,2140,2000,1), DotDir(-3840,0,2000,1));
    triangle->setTexture(imagen);
    triangle->setRgb(rgb(0,0,1.0f));
    figuras.push_back(triangle);
    
    Figure* triangle2 = new Triangle(DotDir(0,2140,2000,1), DotDir(3800,2140,2000,1), DotDir(0,0,2000,1));
    Figure* triangle3 = new Triangle(DotDir(3800,0,2000,1), DotDir(3800,2140,2000,1), DotDir(0,0,2000,1), triangleVertexUV(1,1,1,0,0,1));
    triangle2->setTexture(imagen);
    triangle3->setTexture(imagen);
    figuras.push_back(triangle2);
    figuras.push_back(triangle3);*/

    DotDir center(0,0,-3,1);
    DotDir axis(8,0,0,0);
    DotDir city(0,-4,-3,1);
    if(checkRadius(axis, center, city) ){
        Figure* esfera = new Sphere(center, axis, city);
        esfera->setTexture(imagen);
        figuras.push_back(esfera);
    } else {
        cout << "Error en la esfera." << endl;
    }

    renderer.lanzarRayos(figuras, newImage, 1, 8);
    cout << "Escribo" << endl;
    //tm.equalization(newImage);
    escribirbmp("render5.ppm", newImage, 255);
    for(Figure* i : figuras){
        delete i;
    }

    return 0;
}