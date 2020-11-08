#include <iostream>
#include "../lib/BoundingVolume.h"
#include "../lib/Sensor.h"
#include "../lib/Sphere.h"
#include "../lib/Triangle.h"
#include "../lib/ToneMapper.h"
#include <memory>

void createFaceScene(std::vector<shared_ptr<Figure>>& figuras){
    std::shared_ptr<Figure> plano(new Plane(
                                    DotDir(0, 1, 0, 0),
                                    DotDir(-1, 0, -6, 0), 
                                    DotDir(-20, 5, 0, 1),
                                    30,
                                    150
                                    ));
    plano->setRgb(rgb(255, 0, 0));

    std::shared_ptr<Figure> plano2(new Plane(
                                    DotDir(0, 1, 0, 0),
                                    DotDir(-1, 0, 6, 0), 
                                    DotDir(20, 5, 0, 1),
                                    30,
                                    150
                                    ));
    plano2->setRgb(rgb(0,255,0));

    std::shared_ptr<Figure> plano3(new Plane(
                                    DotDir(0, 0, 1, 0),
                                    DotDir(1, 0, 0, 0), 
                                    DotDir(0, -3, 0, 1),
                                    20,
                                    20
                                    ));
    plano3->setRgb(rgb(0,0,255));

    std::shared_ptr<Figure> plano4(new Plane(
                                    DotDir(0, 1, 0, 0),
                                    DotDir(1, 0, 0, 0), 
                                    DotDir(0, 0, 0, 1),
                                    20,
                                    20
                                    ));
    plano4->setRgb(rgb(124,50,255));
    
    figuras.push_back(plano);
    figuras.push_back(plano2);
    figuras.push_back(plano3);
    figuras.push_back(plano4);

    DotDir center(0,0,-3,1);
    DotDir axis(0,8,0,0);
    DotDir city(0,0,-7,1);
    if(checkRadius(axis, center, city) ){
        std::shared_ptr<Figure> esfera(new Sphere(center, axis, city));
        esfera->setRgb(rgb(255,255,0));
        figuras.push_back(esfera);

    } else {
        cout << "Error en la esfera." << endl;
    }

    DotDir center2(0.6,0.4,-6,1);
    DotDir axis2(0,2,0,0);
    DotDir city2(0.6,0.4,-7,1);
    if(checkRadius(axis2, center2, city2) ){
        std::shared_ptr<Figure> esfera2(new Sphere(center2, axis2, city2));
        esfera2->setRgb(rgb(255,49,134));
        figuras.push_back(esfera2);
    } else {
        cout << "Error en la esfera2." << endl;
    }

    DotDir center3(-0.6,0.4,-6,1);
    DotDir axis3(0,2,0,0);
    DotDir city3(-0.6,0.4,-7,1);
    if(checkRadius(axis3, center3, city3) ){
        std::shared_ptr<Figure> esfera3(new Sphere(center3, axis3, city3));
        esfera3->setRgb(rgb(255,49,134));
        figuras.push_back(esfera3);
    } else {
        cout << "Error en la esfera3." << endl;
    }

    DotDir center4(0,-1,-6,1);
    DotDir axis4(0,2.5,0,0);
    DotDir city4(0,-1,-7.25,1);
    if(checkRadius(axis4, center4, city4) ){
        std::shared_ptr<Figure> esfera4(new Sphere(center4, axis4, city4));
        esfera4->setRgb(rgb(0,0,0));
        figuras.push_back(esfera4);
    } else {
        cout << "Error en la esfera4." << endl;
    }

    // Pusheo un triángulo, las cejas
    std::shared_ptr<Figure> triangle(new Triangle(DotDir(-0.8,0.9,-8,1), DotDir(-0.7,1.2,-8,1), DotDir(-0.1,0.5,-8,1)));
    std::shared_ptr<Figure> triangle2(new Triangle(DotDir(0.1,0.5,-8,1), DotDir(0.7,1.2,-8,1), DotDir(0.8,0.9,-8,1)));
    triangle->setRgb(rgb(255,255,255));
    triangle2->setRgb(rgb(255,255,255));
    figuras.push_back(triangle);
    figuras.push_back(triangle2);
}

int main(){

    float W = 1600, H = 900;
    Image newImage("P3", "IMG", 1600, 900, 255, 255);

    DotDir camera[4];
    camera[0].setDotDir(1, 0, 0, 0);
    camera[1].setDotDir(0, 1, 0, 0);
    camera[2].setDotDir(0, 0, 1, 0);
    camera[3].setDotDir(0, 0, -10, 1);

    Sensor renderer(camera[0], camera[1], camera[2], camera[3], W, H);

    /*std::string file, file2;
    std::cout << "Introduce una imagen: " << std::endl;
    std::cin >> file;
    Image imagen = leer(file);
    ToneMapper tm;
    tm.ReinhardToneMapper(imagen);*/

    vector<shared_ptr<Figure>> figuras;

    std::uniform_real_distribution<float> dist(0.0, 255.0);
    std::default_random_engine gen;
    auto random = std::bind(dist, gen);

    // Pusheo un triángulo, las cejas
    for(int i = 0; i < 100; ++i){
        shared_ptr<Figure> triangle(new Triangle(DotDir(-3840,2140,2000,1), DotDir(0,2140,2000,1), DotDir(-3840,0,2000,1)));
        triangle->setRgb(rgb(random(),random(),random()));
        figuras.push_back(triangle);
    }

    
    //    =================================================
    //    ESCENA DE LA CARA
    //    =============================================

    createFaceScene(figuras);

    /*
    //    ================================================
    //    PLANOS TRIÁNGULOS
    //    ===============================================
    
    shared_ptr<Figure> triangle = new Triangle(DotDir(-3840,2140,2000,1), DotDir(0,2140,2000,1), DotDir(-3840,0,2000,1));
    triangle->setTexture(imagen);
    triangle->setRgb(rgb(0,0,1.0f));
    figuras.push_back(triangle);
    
    shared_ptr<Figure> triangle2 = new Triangle(DotDir(0,2140,2000,1), DotDir(3800,2140,2000,1), DotDir(0,0,2000,1));
    shared_ptr<Figure> triangle3 = new Triangle(DotDir(3800,0,2000,1), DotDir(3800,2140,2000,1), DotDir(0,0,2000,1), triangleVertexUV(1,1,1,0,0,1));
    triangle2->setTexture(imagen);
    triangle3->setTexture(imagen);
    figuras.push_back(triangle2);
    figuras.push_back(triangle3);

    DotDir center(6, 0, -3, 1);
    DotDir axis(0, 8, 0, 0);
    DotDir city(10, 0, -3, 1);
    
    if(checkRadius(axis, center, city) ){
        shared_ptr<Figure> esfera = new Sphere(center, axis, city);
        esfera->setTexture(imagen);
        figuras.push_back(esfera);
    } else {
        cout << "Error en la esfera." << endl;
    }
*/
    renderer.lanzarRayos(figuras, newImage, 8, 8);

    escribir("render5.ppm", newImage, 255);

    //escribir("render.ppm", imagen, 255);


    return 0;
}