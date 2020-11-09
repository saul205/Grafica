#include <iostream>
#include <memory>
#include "../lib/Scene.h"

void createFaceScene(Scene &escena){
    escena.addPlane(DotDir(0, 1, 0, 0), DotDir(-1, 0, -6, 0),   DotDir(-20, 5, 0, 1),   30, 150,    rgb(255, 0, 0));
    escena.addPlane(DotDir(0, 1, 0, 0), DotDir(-1, 0, 6, 0),    DotDir(20, 5, 0, 1),    30, 150,    rgb(0,255,0));
    escena.addPlane(DotDir(0, 0, 1, 0), DotDir(1, 0, 0, 0),     DotDir(0, -3, 0, 1),    20, 20,     rgb(0,0,255));
    //escena.addPlane(DotDir(0, 1, 0, 0), DotDir(1, 0, 0, 0),     DotDir(0, 0, 0, 1),     20, 20,     rgb(124,50,255));
    Image imagen = leer("../practica2/imagenes/seymour_park/seymour_park.ppm");
    ToneMapper tm;
    tm.Filmic(imagen);
    escena.addPlane(DotDir(0,1,0,0), DotDir(1,0,0,0), DotDir(0, 4, 0, 1), 14, 20, imagen);

    escena.addSphere(DotDir(0,0,-3,1), DotDir(0,8,0,0), DotDir(0,0,-7,1), rgb(255,255,0));
    escena.addSphere(DotDir(0.6,0.4,-6,1), DotDir(0,2,0,0), DotDir(0.6,0.4,-7,1), rgb(255,49,134));
    escena.addSphere(DotDir(-0.6,0.4,-6,1), DotDir(0,2,0,0), DotDir(-0.6,0.4,-7,1), rgb(255,49,134));
    escena.addSphere(DotDir(0,-1,-6,1), DotDir(0,2.5,0,0), DotDir(0,-1,-7.25,1), rgb(0,0,0));

    // Pusheo un triángulo, las cejas
    escena.addTriangle(DotDir(-0.8,0.9,-8,1), DotDir(-0.7,1.2,-8,1), DotDir(-0.1,0.5,-8,1), rgb(255,255,255));
    escena.addTriangle(DotDir(0.1,0.5,-8,1), DotDir(0.7,1.2,-8,1), DotDir(0.8,0.9,-8,1), rgb(255,255,255));
}

void createSphereScene(Scene& escena){
    escena.addPlane(DotDir(0, 0, 1, 0), DotDir(1, 0, 0, 0), DotDir(0, -0.5, 20, 0), 100, 100, rgb(255, 153, 255));
    escena.addPlane(DotDir(0, 1, 0, 0), DotDir(1, 0, 0, 0), DotDir(0, 100, 100, 0), 10000, 10000, rgb(51, 255, 255));
    
    std::uniform_real_distribution<float> dist(0.0, 255.0);
    std::default_random_engine gen;
    auto random = std::bind(dist, gen);
    int n = 10;
    int d = 5;
    for(int y = 0; y < 10; y++){
       for(int j = -n * d; j < n * d; j += 5){
            float h = 0;
            for(int i = 1; i < 10; ++i){
                escena.addSphere(DotDir(j,y * 5,h,1), DotDir(0,i,0,0), DotDir(j + i / 2.0f,y * 5,h,1), rgb(random(), random(), random()));
                h += i + 0.5; 
            }
        } 
    }
}

int main(){

    float W = 1600, H = 900;

    DotDir camera[4];
    camera[0].setDotDir(1, 0, 0, 0);
    camera[1].setDotDir(0, 1, 0, 0);
    camera[2].setDotDir(0, 0, 1, 0);
    camera[3].setDotDir(0, 25, -7, 1);

    Scene scene(W, H, camera[0], camera[1], camera[2], camera[3]);

    /*std::uniform_real_distribution<float> dist(0.0, 255.0);
    std::default_random_engine gen;
    auto random = std::bind(dist, gen);

    // Pusheo un triángulo, las cejas
    for(int i = 0; i < 100; ++i){
        scene.addTriangle(DotDir(-3840,2140,2000,1), DotDir(0,2140,2000,1), DotDir(-3840,0,2000,1), rgb(random(),random(),random()));
    }*/

    
    //    =================================================
    //    ESCENA DE LA CARA
    //    =============================================

    //createFaceScene(scene);

    createSphereScene(scene);

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
    // 2000 X 2000 PLANO DE TRIANGULOS
    /*std::uniform_real_distribution<float> dist(0.0, 255.0);
    std::default_random_engine gen;
    auto random = std::bind(dist, gen);

    int n = 10;
    for(int i = -n; i < n; i ++){
        for(int j = n; j > -n; j --){
            scene.addTriangle(DotDir(i, j, 10, 1), DotDir(i + 1, j, 10, 1), DotDir(i, j - 1, 10, 1), rgb(random(), random(), random()));
            scene.addTriangle(DotDir(i + 1, j - 1, 10, 1), DotDir(i + 1, j, 10, 1), DotDir(i, j - 1, 10, 1), rgb(random(), random(), random()));
        }
    }*/

    scene.render("render", 8, 1);

    return 0;
}