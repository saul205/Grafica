#include <iostream>
#include <memory>
#include "../lib/Scene.h"

void createFaceScene(Scene &escena){
    for(int j = 0; j < 10; j++){
        for(int i = 0; i < 10; i++){
            escena.addPlane(DotDir(0, 0, 1, 0), DotDir(1, 0, 0, 0), DotDir(-10 + .5 + i * 2, -3, -.5 - j, 1), 1, 1, rgb(255 * (j % 2), 255 * (j % 2), 255));
            escena.addPlane(DotDir(0, 0, 1, 0), DotDir(1, 0, 0, 0), DotDir(-10 + 1.5 + i * 2, -3, -.5 - j, 1), 1, 1, rgb(255 * ((j + 1) % 2),255 * ((j + 1) % 2),255));
        }
    }

    escena.addPlane(DotDir(0,1,0,0), DotDir(1, 0, 0, 0), DotDir(0, 9, -0.00001, 1), 2, 2, rgb());
    escena.addPlane(DotDir(0, 1, 0, 0), DotDir(0, 0, 1, 0),   DotDir(-10, 4, 0, 1),   14, 10,    rgb(255, 0, 0));
    escena.addPlane(DotDir(0, 1, 0, 0), DotDir(0, 0, 1, 0),    DotDir(10, 4, 0, 1),    14, 10,    rgb(0,255,0));
    //escena.addPlane(DotDir(0, 0, 1, 0), DotDir(1, 0, 0, 0),     DotDir(0, -3, 0, 1),    20, 20,     rgb(0,0,255));
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

void escenaComprobacion(Scene& escena){
    int t = escena.addTriangle(DotDir(0, 0, 0, 1), DotDir(2, 0, 0, 1), DotDir(2,2, 0, 1), rgb(255, 0, 0));
    std::shared_ptr<Figure> triangulo = escena.getFigure(t);
    Transformation tr;
    tr.scale(2, 2, 2);
    triangulo->transform(tr);
}

void cornellBox(Scene& escena){

    std::shared_ptr<Figure> plano(new Plane(DotDir(0,1,0,0), DotDir(-1,0,0,0), DotDir(0,0,4,1), 4, 4));
    plano->setDifBDRF(rgb(0.7,0.7,0.7));
    escena.addPlane(plano);

    //std::shared_ptr<Figure> plano6(new Plane(DotDir(0,1,0,0), DotDir(1,0,0,0), DotDir(0,0,-4.1,1), 4, 4));
    //plano6->setDifBDRF(rgb(0.7,0.7,0.7));
    //escena.addPlane(plano6);

    std::shared_ptr<Figure> plano2(new Plane(DotDir(0,1,0,0), DotDir(0,0,-1,0), DotDir(-2,0,2,1), 4, 4));
    plano2->setDifBDRF(rgb(0.7,0.2,0.2));
    escena.addPlane(plano2);
    
    std::shared_ptr<Figure> plano3(new Plane(DotDir(0,1,0,0), DotDir(0,0,1,0), DotDir(2,0,2,1), 4, 4));
    plano3->setDifBDRF(rgb(0.2,0.7,0.2));
    escena.addPlane(plano3);

    std::shared_ptr<Figure> plano4(new Plane(DotDir(-1,0,0,0), DotDir(0,0,-1,0), DotDir(0,-2,2,1), 4, 4));
    plano4->setDifBDRF(rgb(0.7,0.7,0.7));
    escena.addPlane(plano4);

    escena.addPlane(DotDir(-1,0,0,0), DotDir(0,0,1,0), DotDir(0,1.995,2,1), 2, 2, rgb(10e5,10e5,10e5), true);

    std::shared_ptr<Figure> plano5(new Plane(DotDir(0,0,1,0), DotDir(1,0,0,0), DotDir(0,2,2,1), 4, 4));
    plano5->setDifBDRF(rgb(0.7,0.7,0.7));
    escena.addPlane(plano5);
    
    Image imagen = leer("./IMGS/seymour_park.ppm");

    int x = escena.addSphere(DotDir(1,-1.5,2.5,1), DotDir(0,1,0,0), DotDir(1.5,-1.5,2.5,0), rgb(0,0,0));
    std::shared_ptr<Figure> esfera = escena.getFigure(x);
    esfera->setDifBDRF(rgb(0.2,0.2,0.7));
    //esfera->setTexture(imagen);

    x = escena.addSphere(DotDir(-1,-1.5,3.5,1), DotDir(0,1,0,0), DotDir(-0.5,-1.5,3.5,0), rgb(0,0,0));
    std::shared_ptr<Figure> esfera2 = escena.getFigure(x);
    esfera2->setDifBDRF(rgb(0.7,0.7,0.2));
}

int main(){

    float W = 1500, H = 1500;

    DotDir camera[4];
    camera[0].setDotDir(1, 0, 0, 0);
    camera[1].setDotDir(0, 1, 0, 0);
    camera[2].setDotDir(0, 0, 1, 0);
    camera[3].setDotDir(0, 0, 0, 1);

    Scene scene(W, H, camera[0], camera[1], camera[2], camera[3]);
    // cout << crossProduct(DotDir(0,1,0,0), DotDir(0,0,1,0)).toString() << endl;
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

    //TriangleMesh t;
    //TriangleMesh v;
    //t.read("galleon.ply");
    // v.read("canstick.ply");
    // v.scale(16,16,16);
    //t.move(DotDir(0, 0, 1000, 0));
    // scene.addTriangleMesh(t);
    //scene.addTriangleMesh(v);
    cornellBox(scene);
    scene.render("render", 300, 8);

    return 0;
}