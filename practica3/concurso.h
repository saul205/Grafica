#pragma once

#include "..\lib\Scene.h"

const int anchura = 17.5;
const int altura = 20;

void paredes(Scene& escena){

    //Suelo
    int n = escena.addPlane(DotDir(0, 0, 1, 0), DotDir(-1,0,0,0), DotDir(0, 0, 30, 1), 60, 2 * anchura, rgb());
    escena.getFigure(n)->setDifBRDF(rgb(0.7, 0.1 ,0.7));

    //Fondo
    n = escena.addPlane(DotDir(0, 1, 0, 0), DotDir(-1,0,0,0), DotDir(0, altura / 2.f, 60, 1), altura, 2 * anchura, rgb());
    escena.getFigure(n)->setDifBRDF(rgb(0.7, 0.7 ,0.1));

    //Paredes
    n = escena.addPlane(DotDir(0, 1, 0, 0), DotDir(0,0,-1,0), DotDir(-anchura, altura / 2.f, 30, 1), altura, 60, rgb());
    escena.getFigure(n)->setDifBRDF(rgb(0.1, 0.7 ,0.7));
    n = escena.addPlane(DotDir(0, 1, 0, 0), DotDir(0,0,1,0), DotDir(anchura, altura / 2.f, 30, 1), altura, 60, rgb());
    escena.getFigure(n)->setDifBRDF(rgb(0.1, 0.7 ,0.7));
}

void pilares(Scene& escena){

}

void escenaConcurso(Scene& escena){
    paredes(escena);

    escena.addPlane(DotDir(0, 0, -1, 0), DotDir(-1,0,0,0), DotDir(0, 20, 50, 1), 100, 60, rgb(50,50,50), true);
}