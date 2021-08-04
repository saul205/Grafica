#include <iostream>
#include <memory>
#include <cstring>
#include "../lib/Scene.h"

const int anchura = 17.5;
const int altura = 20;

void paredes(Scene& escena){

    //Suelo
    Image imagen = leer("suelo.ppm");
    int n = escena.addPlane(DotDir(0, 0, 1, 0), DotDir(-1,0,0,0), DotDir(0, 0, 30, 1), 60, 2 * anchura, imagen);
    escena.getFigure(n)->setDifBRDF(rgb(0.9, 0.9 ,0.9));

    n = escena.addPlane(DotDir(0, 0, 1, 0), DotDir(-1,0,0,0), DotDir(0, 0, 15, 1), 30, 10, rgb());
    escena.getFigure(n)->setDifBRDF(rgb(0.5, 0.1 ,0.1));

    n = escena.addPlane(DotDir(0, 0, -1, 0), DotDir(-1,0,0,0), DotDir(12.25, 20, 30, 1), 60, 10.5, rgb());
    escena.getFigure(n)->setDifBRDF(rgb(0.7, 0.7 ,0.7));

    n = escena.addPlane(DotDir(0, 0, -1, 0), DotDir(-1,0,0,0), DotDir(-12.25, 20, 30, 1), 60, 10.5, rgb());
    escena.getFigure(n)->setDifBRDF(rgb(0.7, 0.7 ,0.7));

    //Fondo
    n = escena.addPlane(DotDir(0, 1, 0, 0), DotDir(-1,0,0,0), DotDir(0, altura / 2.f, 60.01, 1), altura, 2 * anchura, rgb());
    escena.getFigure(n)->setDifBRDF(rgb(0.58,0.00,0.70));

    //Paredes
    n = escena.addPlane(DotDir(0, 1, 0, 0), DotDir(0,0,-1,0), DotDir(-anchura, altura / 2.f, 55, 1), altura, 10, rgb());
    escena.getFigure(n)->setDifBRDF(rgb(0.1, 0.7 ,0.7));
    n = escena.addPlane(DotDir(0, 1, 0, 0), DotDir(0,0,-1,0), DotDir(-anchura, altura / 2.f, 15, 1), altura, 30, rgb());
    escena.getFigure(n)->setDifBRDF(rgb(0.1, 0.7 ,0.7));
    n = escena.addPlane(DotDir(0, 1, 0, 0), DotDir(0,0,-1,0), DotDir(-anchura, 18, 40, 1), 4, 20, rgb());
    escena.getFigure(n)->setDifBRDF(rgb(0.1, 0.7 ,0.7));
    n = escena.addPlane(DotDir(0, 1, 0, 0), DotDir(0,0,-1,0), DotDir(-anchura, 3, 40, 1), 6, 20, rgb());
    escena.getFigure(n)->setDifBRDF(rgb(0.1, 0.7 ,0.7));

    n = escena.addPlane(DotDir(0, 1, 0, 0), DotDir(0,0,1,0), DotDir(anchura, altura / 2.f, 55, 1), altura, 10, rgb());
    escena.getFigure(n)->setDifBRDF(rgb(0.1, 0.7 ,0.7));
    n = escena.addPlane(DotDir(0, 1, 0, 0), DotDir(0,0,1,0), DotDir(anchura, altura / 2.f, 15, 1), altura, 30, rgb());
    escena.getFigure(n)->setDifBRDF(rgb(0.1, 0.7 ,0.7));
    n = escena.addPlane(DotDir(0, 1, 0, 0), DotDir(0,0,-1,0), DotDir(anchura, 18, 40, 1), 4, 20, rgb());
    escena.getFigure(n)->setDifBRDF(rgb(0.1, 0.7 ,0.7));
    n = escena.addPlane(DotDir(0, 1, 0, 0), DotDir(0,0,-1,0), DotDir(anchura, 3, 40, 1), 6, 20, rgb());
    escena.getFigure(n)->setDifBRDF(rgb(0.1, 0.7 ,0.7));

    //trifuerza izq
    n = escena.addTriangle(DotDir(-anchura, 6, 50, 1), DotDir(-anchura, 11, 45, 1), DotDir(-anchura, 6, 40, 1));
    escena.getFigure(n)->setSpecBRDF(rgb(0.8,0.8,0.8));
    n = escena.addTriangle(DotDir(-anchura, 11, 45, 1), DotDir(-anchura, 11, 35, 1), DotDir(-anchura, 6, 40, 1));
    escena.getFigure(n)->setSpecBRDF(rgb(0.5,0.5,0.5));
    n = escena.addTriangle(DotDir(-anchura, 11, 45, 1), DotDir(-anchura, 16, 40, 1), DotDir(-anchura, 11, 35, 1));
    escena.getFigure(n)->setSpecBRDF(rgb(0.8,0.8,0.8));
    n = escena.addTriangle(DotDir(-anchura, 6, 40, 1), DotDir(-anchura, 11, 35, 1), DotDir(-anchura, 6, 30, 1));
    escena.getFigure(n)->setSpecBRDF(rgb(0.8,0.8,0.8));

    //trifuerza izq
    n = escena.addTriangle(DotDir(anchura, 6, 50, 1), DotDir(anchura, 11, 45, 1), DotDir(anchura, 6, 40, 1));
    escena.getFigure(n)->setSpecBRDF(rgb(0.8,0.8,0.8));
    n = escena.addTriangle(DotDir(anchura, 11, 45, 1), DotDir(anchura, 11, 35, 1), DotDir(anchura, 6, 40, 1));
    escena.getFigure(n)->setSpecBRDF(rgb(0.5,0.5,0.5));
    n = escena.addTriangle(DotDir(anchura, 11, 45, 1), DotDir(anchura, 16, 40, 1), DotDir(anchura, 11, 35, 1));
    escena.getFigure(n)->setSpecBRDF(rgb(0.8,0.8,0.8));
    n = escena.addTriangle(DotDir(anchura, 6, 40, 1), DotDir(anchura, 11, 35, 1), DotDir(anchura, 6, 30, 1));
    escena.getFigure(n)->setSpecBRDF(rgb(0.8,0.8,0.8));
}

void cuadro(Scene& escena){

    Image imagen = leer("Touched_by_His_Noodly_Appendage_HD.ppm");

    // Cuadro
    int x = escena.addPlane(DotDir(0,1,0,0), DotDir(1,0,0,0), DotDir(0, altura / 2.f, 59.995,1), altura / 2.0, anchura , imagen);
    std::shared_ptr<Figure> plano6 = escena.getFigure(x);
    plano6->setDifBRDF(rgb(1,1,1));

    // Bordes
    //escena.addPlane(DotDir(-1,0,0,0), DotDir(0,1,0,0), DotDir(0,altura / 2.0f, 59.99,1), anchura + 1, altura / 2.0f + 1, rgb(500,500,500), true);
    escena.addPlane(DotDir(-1,0,0,0), DotDir(0,0,-1,0), DotDir(0,3* altura / 4.0f,59.995,1), anchura, 3, rgb(1000,1000,1000), true);
    escena.addPlane(DotDir(-1,0,0,0), DotDir(0,0,-1,0), DotDir(0,altura / 4.0f ,59.995,1), anchura, 3, rgb(100,1000,1000), true);
    escena.addPlane(DotDir(-1,0,0,0), DotDir(0,0,-1,0), DotDir(0,altura / 4.0f - 0.1,59.995,1), anchura, 3, rgb(1000,1000,1000), true);
    escena.addPlane(DotDir(-1,0,0,0), DotDir(0,0,-1,0), DotDir(0,altura / 4.0f - 0.2,59.995,1), anchura, 3, rgb(1000,1000,1000), true);
    escena.addPlane(DotDir(-1,0,0,0), DotDir(0,0,-1,0), DotDir(0,altura / 4.0f - 0.05,59.995,1), anchura, 3, rgb(1000,1000,1000), true);
    escena.addPlane(DotDir(-1,0,0,0), DotDir(0,0,-1,0), DotDir(0,altura / 4.0f - 0.15,59.995,1), anchura, 3, rgb(1000,1000,1000), true);
    escena.addPlane(DotDir(0,-1,0,0), DotDir(0,0,-1,0), DotDir(anchura / 2.0f,altura / 2.0,59.995,1), altura / 2.0, 3, rgb(1000,1000,1000), true);
    escena.addPlane(DotDir(0,-1,0,0), DotDir(0,0,-1,0), DotDir(-anchura /2.0f,altura / 2.0,59.995,1), altura / 2.0, 3, rgb(1000,1000,1000), true);

}

void esferas(Scene& escena){
    int x = escena.addSphere(DotDir(-13, 1, 14,1), DotDir(0,2,0,0), DotDir(-12, 1, 14,1), rgb(0,0,0));
    std::shared_ptr<Figure> esfera = escena.getFigure(x);
    esfera->setDielectrico();
    esfera->refractionIndex = 1.55;

    x = escena.addSphere(DotDir(-5, 1, 27,1), DotDir(0,2,0,0), DotDir(-4, 1, 27,1), rgb(0,0,0));
    esfera = escena.getFigure(x);
    esfera->setDielectrico();
    esfera->refractionIndex = 1.55;

    x = escena.addSphere(DotDir(4, 1, 40,1), DotDir(0,2,0,0), DotDir(5, 1, 40,1), rgb(0,0,0));
    esfera = escena.getFigure(x);
    esfera->setDielectrico();
    esfera->refractionIndex = 1.55;

    x = escena.addSphere(DotDir(11, 1, 20,1), DotDir(0,2,0,0), DotDir(10, 1, 20,1), rgb(0,0,0));
    esfera = escena.getFigure(x);
    esfera->setDielectrico();
    esfera->refractionIndex = 1.55;
}

void pilares(Scene& escena){

    TriangleMesh t;
    t.read("canstick.ply");
    t.scale(35, 35, 35);
    int x = 0;


    // Lineas de luz del suelo
    escena.addPlane(DotDir(1,0,0,0), DotDir(0,0,1,0), DotDir(-7.5, 0, 0,1), 0.3, 210, rgb(1000,1000,1000), true);
    escena.addPlane(DotDir(1,0,0,0), DotDir(0,0,1,0), DotDir(-10.5, 0, 0,1), 0.3, 210, rgb(1000,1000,1000), true);
    escena.addPlane(DotDir(1,0,0,0), DotDir(0,0,1,0), DotDir(7.5, 0, 0,1), 0.3, 210, rgb(1000,1000,1000), true);
    escena.addPlane(DotDir(1,0,0,0), DotDir(0,0,1,0), DotDir(10.5, 0, 0,1), 0.3, 210, rgb(1000,1000,1000), true);

    escena.addLight(DotDir(-9, 1, 10, 1), rgb(163*15,73*15,164*15));
    escena.addLight(DotDir(-9, 1, 20, 1), rgb(163*15,73*15,164*15));
    escena.addLight(DotDir(-9, 1, 30, 1), rgb(163*15,73*15,164*15));
    escena.addLight(DotDir(-9, 1, 40, 1), rgb(163*15,73*15,164*15));
    escena.addLight(DotDir(9, 1, 10, 1), rgb(163*15,73*15,164*15));
    escena.addLight(DotDir(9, 1, 20, 1), rgb(163*15,73*15,164*15));
    escena.addLight(DotDir(9, 1, 30, 1), rgb(163*15,73*15,164*15));
    escena.addLight(DotDir(9, 1, 40, 1), rgb(163*15,73*15,164*15));

    x = escena.addSphere(DotDir(-9, altura / 4.0 + 0.75, 25,1), DotDir(0,1.5,0,0), DotDir(-9.75,altura / 4.0  + 0.75, 25,0), rgb(0,0,0));
    std::shared_ptr<Figure> esfera = escena.getFigure(x);
    esfera->setRefBRDF(rgb(0.7,0.7,0.7));
    esfera->refractionIndex = 1.55;
    t.move(DotDir(-9, 1.9, 25, 1));
    escena.addTriangleMesh(t);

    x = escena.addSphere(DotDir(9, altura / 4.0 + 0.75, 25,1), DotDir(0,1.5,0,0), DotDir(9.75,altura / 4.0  + 0.75, 25,0), rgb(0,0,0));
    std::shared_ptr<Figure> esfera2 = escena.getFigure(x);
    esfera2->setRefBRDF(rgb(0.7,0.7,0.7));
    esfera2->refractionIndex = 1.55;
    t.move(DotDir(18, 0, 0, 1));
    escena.addTriangleMesh(t);

    x = escena.addSphere(DotDir(9, altura / 4.0 + 0.75, 35,1), DotDir(0,1.5,0,0), DotDir(9.75,altura / 4.0  + 0.75, 35,0), rgb(0,0,0));
    std::shared_ptr<Figure> esfera3 = escena.getFigure(x);
    esfera3->setSpecBRDF(rgb(0.9,0.9,0.9));
    t.move(DotDir(0, 0, 10, 1));
    escena.addTriangleMesh(t);

    x = escena.addSphere(DotDir(-9, altura / 4.0 + 0.75, 35,1), DotDir(0,1.5,0,0), DotDir(-9.75,altura / 4.0  + 0.75, 35,0), rgb(0,0,0));
    std::shared_ptr<Figure> esfera4 = escena.getFigure(x);
    esfera4->setSpecBRDF(rgb(0.9,0.9,0.9));
    t.move(DotDir(-18, 0, 0, 1));
    escena.addTriangleMesh(t);

    x = escena.addSphere(DotDir(-9, altura / 4.0 + 0.75, 45,1), DotDir(0,1.5,0,0), DotDir(-9.75,altura / 4.0  + 0.75, 45,0), rgb(rgb(255,25,255)), true);
    std::shared_ptr<Figure> esfera5 = escena.getFigure(x);
    t.move(DotDir(0, 0, 10, 1));
    escena.addTriangleMesh(t);

    x = escena.addSphere(DotDir(9, altura / 4.0 + 0.75, 45,1), DotDir(0,1.5,0,0), DotDir(9.75,altura / 4.0  + 0.75, 45,0), rgb(rgb(255,25,255)), true);
    std::shared_ptr<Figure> esfera6 = escena.getFigure(x);
    t.move(DotDir(18, 0, 0, 1));
    escena.addTriangleMesh(t);

    x = escena.addSphere(DotDir(9, altura / 4.0 + 0.75, 15,1), DotDir(0,1.5,0,0), DotDir(9.75,altura / 4.0  + 0.75, 15,0), rgb(85,255,51), true);
    std::shared_ptr<Figure> esfera7 = escena.getFigure(x);
    t.move(DotDir(0, 0, -30, 1));
    escena.addTriangleMesh(t);

    x = escena.addSphere(DotDir(-9, altura / 4.0 + 0.75, 15,1), DotDir(0,1.5,0,0), DotDir(-9.75,altura / 4.0  + 0.75, 15,0), rgb(85,255,51), true);
    std::shared_ptr<Figure> esfera8 = escena.getFigure(x);
    t.move(DotDir(-18, 0, 0, 1));
    escena.addTriangleMesh(t);

    // Central
    // escena.addLight(DotDir(0, altura / 5.0, 37, 1), rgb(5000,5000,5000));
    // escena.addLight(DotDir(0, altura / 4.0, 37, 1), rgb(5000,5000,5000));
    x = escena.addSphere(DotDir(0, altura / 3.0 - 1.5, 37,1), DotDir(0,5,0,0), DotDir(-2.5,altura / 3.0 - 1.5, 37,0), rgb(0, 0, 0));
    std::shared_ptr<Figure> esfera9 = escena.getFigure(x);
    esfera9->setSpecBRDF(rgb(1,1,1));
    t.move(DotDir(9,-4,0,1));
    t.scale(2.5,2.5,2.5);
    escena.addTriangleMesh(t);

}


void cornellBoxBasica(Scene& escena){

    paredes(escena);
    cuadro(escena);
    pilares(escena);
    esferas(escena);

    Image imagen1 = leer("milky-way-and-starry-night-sky.ppm");
    Image imagen2 = leer("milky-way-star-night-starry-sky-wallpaper.ppm");
    // ESto que es??
    int n = escena.addPlane(DotDir(0, 0, -1, 0), DotDir(-1,0,0,0), DotDir(0, 30, 30, 1), 120, 60, imagen2);
    escena.getFigure(n)->setDifBRDF(rgb(0.9,0.9,0.9));
    n = escena.addPlane(DotDir(0, 1, 0, 0), DotDir(-1,0,0,0), DotDir(0, 15, 70, 1), 30, 35, imagen1);
    escena.getFigure(n)->setDifBRDF(rgb(0.9,0.9,0.9));
    n = escena.addPlane(DotDir(0, 1, 0, 0), DotDir(0,0,-1,0), DotDir(-anchura - 10, 15, 80, 1), 30, 160, imagen2);
    escena.getFigure(n)->setDifBRDF(rgb(0.9,0.9,0.9));
    n = escena.addPlane(DotDir(0, 1, 0, 0), DotDir(0,0,1,0), DotDir(anchura + 10, 15, 80, 1), 30, 160, imagen2);
    escena.getFigure(n)->setDifBRDF(rgb(0.9,0.9,0.9));

    escena.addSphere(DotDir(anchura-5, 25, 30, 1), DotDir(0, 10, 0, 0), DotDir(anchura-10, 25, 30, 1), rgb(5000, 5000, 5000), true);
}

int main(int argc, char** argv){

    if(argc < 2 || argc > 12){
        cout << "Formato de ejecución: main <ppp> [-width] [-height] [-name] [-color] [-threads]" << endl;
        exit(0);
    }

    float W = 1920, H = 1080;
    float col_res = 1e9;
    string name = "render";
    int threads = 6;

    for(int i = 2; i < argc; i++){
        if(!strcmp(argv[i], "-width")){++i; W = atof(argv[i]);}
        else if(!strcmp(argv[i], "-height")){++i; H = atof(argv[i]);}
        else if(!strcmp(argv[i], "-name")){++i; name = argv[i]; }
        else if(!strcmp(argv[i], "-color")){++i; col_res = atof(argv[i]);}
        else if(!strcmp(argv[i], "-threads")){++i; threads = atoi(argv[i]);}
        else{++i;}
    }

    Scene scene(W, H, DotDir(0,7.5,0,1), DotDir(0,0,15,0), col_res);    
    cornellBoxBasica(scene);
    cout << name << endl;
    scene.render(name, atoi(argv[1]), threads);

    return 0;
}
