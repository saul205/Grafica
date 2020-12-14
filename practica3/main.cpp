#include <iostream>
#include <memory>
#include <cstring>
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
    //Image imagen = leer("../practica2/imagenes/seymour_park/seymour_park.ppm");
    //ToneMapper tm;
    //tm.Filmic(imagen);
    //escena.addPlane(DotDir(0,1,0,0), DotDir(1,0,0,0), DotDir(0, 4, 0, 1), 14, 20, imagen);

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

    std::shared_ptr<Figure> plano2(new Plane(DotDir(0,1,0,0), DotDir(0,0,1,0), DotDir(2,0,2,1), 4, 4));
    plano2->setDifBDRF(rgb(1,0.2,0.2));
    escena.addPlane(plano2);
    
    std::shared_ptr<Figure> plano3(new Plane(DotDir(0,1,0,0), DotDir(0,0,-1,0), DotDir(-2,0,2,1), 4, 4));
    plano3->setDifBDRF(rgb(0.2,1,0.2));
    escena.addPlane(plano3);

    std::shared_ptr<Figure> plano4(new Plane(DotDir(-1,0,0,0), DotDir(0,0,-1,0), DotDir(0,-2,2,1), 4, 4));
    plano4->setDifBDRF(rgb(0.7,0.7,0.7));
    escena.addPlane(plano4);

    escena.addPlane(DotDir(-1,0,0,0), DotDir(0,0,1,0), DotDir(0,1.995,2,1), 2, 2, rgb(10e5,10e5,10e5), true);

    std::shared_ptr<Figure> plano5(new Plane(DotDir(0,0,1,0), DotDir(1,0,0,0), DotDir(0,2,2,1), 4, 4));
    plano5->setDifBDRF(rgb(0.7,0.7,0.7));
    escena.addPlane(plano5);

    int x = 0;

    x = escena.addSphere(DotDir(1,-1.5,1.5,1), DotDir(0,1,0,0), DotDir(0.5,-1.5,1.5,0), rgb(0,0,0));
    std::shared_ptr<Figure> esfera = escena.getFigure(x);
    esfera->setDielectrico();
    esfera->refractionIndex = 1.55;
    //esfera->setTexture(imagen);

    //x = escena.addSphere(DotDir(0,-1,2,1), DotDir(0,1,0,0), DotDir(-0.5,-1,2,0), rgb(0,0,0));
    //std::shared_ptr<Figure> esfera2 = escena.getFigure(x);
    //esfera2->setSpecBDRF(rgb(0.7,0.7,0.7));

    Image imagen = leer("seymour_park.ppm");
    ToneMapper tm;
    tm.Filmic(imagen);
    x = escena.addSphere(DotDir(1,1,3,1), DotDir(0,1,0,0), DotDir(0.5,1,3,0), imagen);
    std::shared_ptr<Figure> esfera3 = escena.getFigure(x);
    esfera3->setDifBDRF(rgb(0.7,0.7,0.7));

    x = escena.addSphere(DotDir(-1,1,1,1), DotDir(0,1,0,0), DotDir(-1.5,1,1,0), rgb(0,0,0));
    std::shared_ptr<Figure> esfera4 = escena.getFigure(x);
    esfera4->setDifBDRF(rgb(0.2,0.2,0.7));
    esfera4->setSpecBDRF(rgb(0.2,0.2,0.2));

    //x = escena.addSphere(DotDir(-1,-1.5,1,1), DotDir(0,1,0,0), DotDir(-1.5,-1.5,1,0), rgb(0,0,0));
    //std::shared_ptr<Figure> esfera5 = escena.getFigure(x);
    //esfera5->setSpecBDRF(rgb(0.7,0.7,0.7));

    // int l = escena.addLight(DotDir(0, -1.75, 2, 1), rgb(10e5, 10e5, 0));
}

void cornellBox2(Scene& escena){

    cornellBox(escena);

    Image imagen = leer("seymour_park.ppm");
    ToneMapper tm;
    tm.Filmic(imagen);
    //int n = escena.addPlane(DotDir(0,1,0,0), DotDir(1,0,0,0), DotDir(0,0,3,1), 3, 3, imagen);
    //escena.getFigure(n)->setDifBDRF(rgb(1,1,1));


    int n = escena.addTriangle(DotDir(0.5, 0.5, 2, 1), DotDir(-0.5, 0.5, 2, 1), DotDir(0.5, -0.5, 2, 1), imagen);
    escena.getFigure(n)->setDifBDRF(rgb(0.7, 0.7, 0.4));
    triangleVertexUV t(1, 1, 0, 1, 1, 0);
    n = escena.addTriangle(DotDir(-0.5, -0.5, 2, 1), DotDir(0.5, -0.5, 2, 1), DotDir(-0.5, 0.5, 2, 1), imagen, t);
    escena.getFigure(n)->setDifBDRF(rgb(0.7, 0.7, 0.4));
}

void cornellBoxBasica(Scene& escena){

    std::shared_ptr<Figure> plano(new Plane(DotDir(0,1,0,0), DotDir(-1,0,0,0), DotDir(0,0,4,1), 4, 4));
    plano->setDifBDRF(rgb(0.7,0.7,0.7));
    escena.addPlane(plano);

    std::shared_ptr<Figure> plano2(new Plane(DotDir(0,1,0,0), DotDir(0,0,1,0), DotDir(2,0,2,1), 4, 4));
    plano2->setDifBDRF(rgb(1,0.2,0.2));
    escena.addPlane(plano2);

    std::shared_ptr<Figure> plano3(new Plane(DotDir(0,1,0,0), DotDir(0,0,-1,0), DotDir(-2,0,2,1), 4, 4));
    plano3->setDifBDRF(rgb(0.2,1,0.2));
    escena.addPlane(plano3);

    std::shared_ptr<Figure> plano4(new Plane(DotDir(-1,0,0,0), DotDir(0,0,-1,0), DotDir(0,-2,2,1), 4, 4));
    plano4->setDifBDRF(rgb(0.7,0.7,0.7));
    escena.addPlane(plano4);

    escena.addPlane(DotDir(-1,0,0,0), DotDir(0,0,1,0), DotDir(0,1.995,2,1), 2, 2, rgb(50,50,50), true);

    std::shared_ptr<Figure> plano5(new Plane(DotDir(0,0,1,0), DotDir(1,0,0,0), DotDir(0,2,2,1), 4, 4));
    plano5->setDifBDRF(rgb(0.7,0.7,0.7));
    escena.addPlane(plano5);

    int x = 0;
    x = escena.addSphere(DotDir(1,-1.5,1.5,1), DotDir(0,1,0,0), DotDir(0.5,-1.5,1.5,0), rgb(0,0,0));
    std::shared_ptr<Figure> esfera = escena.getFigure(x);
    esfera->setDielectrico();
    esfera->refractionIndex = 1.55;

    x = escena.addSphere(DotDir(-1,-1.5,3,1), DotDir(0,1,0,0), DotDir(-1.5,-1.5,3,0), rgb(0,0,0));
    std::shared_ptr<Figure> esfera4 = escena.getFigure(x);
    esfera4->setDifBDRF(rgb(0.2,0.2,0.7));
    esfera4->setSpecBDRF(rgb(0.2,0.2,0.2));
}

void triangleScene(Scene& escena){

    std::shared_ptr<Figure> plano(new Plane(DotDir(0,1,0,0), DotDir(-1,0,0,0), DotDir(0,0,4,1), 4, 4));
    plano->setDifBDRF(rgb(0.7,0.7,0.7));
    escena.addPlane(plano);

    std::shared_ptr<Figure> plano2(new Plane(DotDir(0,1,0,0), DotDir(0,0,1,0), DotDir(2,0,2,1), 4, 4));
    plano2->setDifBDRF(rgb(1,0.2,0.2));
    escena.addPlane(plano2);

    std::shared_ptr<Figure> plano3(new Plane(DotDir(0,1,0,0), DotDir(0,0,-1,0), DotDir(-2,0,2,1), 4, 4));
    plano3->setDifBDRF(rgb(0.2,1,0.2));
    escena.addPlane(plano3);

    std::shared_ptr<Figure> plano4(new Plane(DotDir(-1,0,0,0), DotDir(0,0,-1,0), DotDir(0,-2,2,1), 4, 4));
    plano4->setDifBDRF(rgb(0.7,0.7,0.7));
    escena.addPlane(plano4);

    escena.addPlane(DotDir(-1,0,0,0), DotDir(0,0,1,0), DotDir(0,1.995,2,1), 2, 2, rgb(50,50,50), true);

    std::shared_ptr<Figure> plano5(new Plane(DotDir(0,0,1,0), DotDir(1,0,0,0), DotDir(0,2,2,1), 4, 4));
    plano5->setDifBDRF(rgb(0.7,0.7,0.7));
    escena.addPlane(plano5);

    TriangleMesh t;
    t.read("canstick.ply");
    t.scale(10, 10, 10);
    t.move(DotDir(1, 0, 1, 1));
    escena.addTriangleMesh(t);
}

int main(int argc, char** argv){

    if(argc < 2 || argc > 10){
        cout << "Formato de ejecución: main <ppp> [-width] [-height] [-name] [-color] [-threads]" << endl;
        exit(0);
    }

    float W = 1080, H = 1080;
    float col_res = 1e5;
    string name = "render";
    int threads = 1;

    for(int i = 2; i < argc; i++){
        if(!strcmp(argv[i], "-width")){++i; W = atof(argv[i]);}
        else if(!strcmp(argv[i], "-height")){++i; H = atof(argv[i]);}
        else if(!strcmp(argv[i], "-name")){++i; name = argv[i];}
        else if(!strcmp(argv[i], "-color")){++i; col_res = atof(argv[i]);}
        else if(!strcmp(argv[i], "-threads")){++i; threads = atoi(argv[i]);}
        else{++i;}
    }

    Scene scene(W, H, DotDir(0,0,1,1), DotDir(0,0,8,0), col_res);

    triangleScene(scene);
    scene.render(name, atoi(argv[1]), threads);

    return 0;
}
