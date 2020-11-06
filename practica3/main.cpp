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
    camera[3].setDotDir(0, 0, 0, 1);

    Sensor renderer(camera[0], camera[1], camera[2], camera[3], W, H);

    vector<Figure*> figuras;

    std::uniform_real_distribution<float> dist(0.0, 255.0);
    std::default_random_engine gen;
    auto random = std::bind(dist, gen);

    // Pusheo un triángulo, las cejas
    for(int i = 0; i < 100; ++i){
        Figure* triangle = new Triangle(DotDir(-3840,2140,2000,1), DotDir(0,2140,2000,1), DotDir(-3840,0,2000,1));
        triangle->setRgb(rgb(random(),random(),random()));
        figuras.push_back(triangle);
    }

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

    renderer.lanzarRayos(figuras, newImage, 20, 6);
    cout << "Escribo" << endl;
    //tm.equalization(newImage);
    escribirbmp("render5.ppm", newImage, 255);
    for(Figure* i : figuras){
        delete i;
    }

    return 0;
}