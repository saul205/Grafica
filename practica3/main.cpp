#include <iostream>
#include <memory>
#include <cstring>
#include "../lib/Scene.h"


void texturas(Scene& escena){

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

    escena.addPlane(DotDir(-1,0,0,0), DotDir(0,0,1,0), DotDir(0,1.995,2,1), 2, 2, rgb(10e5,10e5,10e5), true);

    std::shared_ptr<Figure> plano5(new Plane(DotDir(0,0,1,0), DotDir(1,0,0,0), DotDir(0,2,2,1), 4, 4));
    plano5->setDifBDRF(rgb(0.7,0.7,0.7));
    escena.addPlane(plano5);

    Image imagen = leer("seymour_park.ppm");
    ToneMapper tm;
    tm.gammaCurveAndClamping(imagen, imagen.getMaximo() * 0.15, 1 / 2.2f);

    int x = escena.addPlane(DotDir(0,1,0,0), DotDir(0,0,1,0), DotDir(-1.999,0,2,1), 2, 2, imagen);
    std::shared_ptr<Figure> plano6 = escena.getFigure(x);
    plano6->setDifBDRF(rgb(0.7,0.7,0.7));
    escena.addPlane(plano6);

    x = escena.addSphere(DotDir(-1,-1.5,3,1), DotDir(0,1,0,0), DotDir(-1.5,-1.5,3,0), imagen);
    std::shared_ptr<Figure> esfera3 = escena.getFigure(x);
    esfera3->setDifBDRF(rgb(0.7,0.7,0.7));
    x = escena.addSphere(DotDir(1,-1.5,1.5,1), DotDir(0,1,0,0), DotDir(0.5,-1.5,1.5,0), imagen);
    std::shared_ptr<Figure> esfera4 = escena.getFigure(x);
    esfera4->setDifBDRF(rgb(0.7,0.7,0.7));

    triangleVertexUV t1(0, 0, 0.5, 0, 0, 1);
    int n = escena.addTriangle(DotDir(1, 0.5, 2, 1), DotDir(0.1, 0.5, 2, 1), DotDir(1, -0.5, 2, 1), imagen, t1);
    escena.getFigure(n)->setDifBDRF(rgb(0.7, 0.7, 0.7));
    triangleVertexUV t2(1, 0, 0.5, 0, 1, 1);
    n = escena.addTriangle(DotDir(-1, 0.5, 2, 1), DotDir(-0.1, 0.5, 2, 1), DotDir(-1, -0.5, 2, 1), imagen, t2);
    escena.getFigure(n)->setDifBDRF(rgb(0.7, 0.7, 0.7));
    triangleVertexUV t3(1, 1, 0.5, 0, 0, 1);
    n = escena.addTriangle(DotDir(-0.9, -0.5, 2, 1), DotDir(0, 0.5, 2, 1), DotDir(0.9, -0.5, 2, 1), imagen, t3);
    escena.getFigure(n)->setDifBDRF(rgb(0.7, 0.7, 0.7));
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

    escena.addPlane(DotDir(-1,0,0,0), DotDir(0,0,1,0), DotDir(0,1.995,2,1), 2, 2, rgb(10e5,10e5,10e5), true);

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

void cornellBoxEspejos(Scene& escena){
    std::shared_ptr<Figure> plano(new Plane(DotDir(0,1,0,0), DotDir(-1,0,0,0), DotDir(0,0,4,1), 4, 4));
    plano->setDifBDRF(rgb(0.7,0.7,0.7));
    escena.addPlane(plano);

    std::shared_ptr<Figure> plano2(new Plane(DotDir(0,1,0,0), DotDir(0,0,1,0), DotDir(2,0,2,1), 4, 4));
    plano2->setSpecBDRF(rgb(0.7,0.7,0.7));
    plano2->setDifBDRF(rgb(1,0.2,0.2));
    escena.addPlane(plano2);

    std::shared_ptr<Figure> plano3(new Plane(DotDir(0,1,0,0), DotDir(0,0,-1,0), DotDir(-2,0,2,1), 4, 4));
    plano3->setSpecBDRF(rgb(0.7,0.7,0.7));
    plano3->setDifBDRF(rgb(0.2,1,0.2));
    escena.addPlane(plano3);

    std::shared_ptr<Figure> plano4(new Plane(DotDir(-1,0,0,0), DotDir(0,0,-1,0), DotDir(0,-2,2,1), 4, 4));
    plano4->setDifBDRF(rgb(0.7,0.7,0.7));
    escena.addPlane(plano4);

    escena.addPlane(DotDir(-1,0,0,0), DotDir(0,0,1,0), DotDir(0,1.995,2,1), 2, 2, rgb(10e5,10e5,10e5), true);

    std::shared_ptr<Figure> plano5(new Plane(DotDir(0,0,1,0), DotDir(1,0,0,0), DotDir(0,2,2,1), 4, 4));
    plano5->setSpecBDRF(rgb(0.7,0.7,0.7));
    plano5->setDifBDRF(rgb(0.3,0.3,0.3));
    escena.addPlane(plano5);

    int x = 0;
    x = escena.addSphere(DotDir(1,-1.5,1.5,1), DotDir(0,1,0,0), DotDir(0.5,-1.5,1.5,0), rgb(10e5,10e5,10e5), true);

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

    cornellBoxEspejos(scene);
    scene.render(name, atoi(argv[1]), threads);

    return 0;
}
