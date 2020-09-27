#include "Sphere.h"
#include <iostream>

using namespace std;

int main(){

    DotDir dd1(0,5,3,1);
    DotDir dd2(2,0,1,0);
    DotDir dd3(4,0,0,0);
    DotDir dd4 = dd2 + dd3;
    DotDir dd5 = dd2 + dd1;
    DotDir dd6 = dd1 + dd2;
    DotDir dd7 = dd2 - dd3;
    DotDir dd8 = dd1 - dd1;
    DotDir dd9 = dd1 - dd2;

    cout << "Dirección DD1: " << dd1.toString() << " módulo: " << dd1.mod() << endl;
    cout << "Dirección DD2: " << dd2.toString() << " módulo: " << dd2.mod() << endl;
    cout << "Dirección DD3: " << dd3.toString() << " módulo: " << dd3.mod() << endl;
    cout << "Dirección más dirección (dd2 + dd3): " << dd4.toString() << " módulo: " << dd4.mod() << endl;
    cout << "Dirección más punto (dd2 + dd1):     " << dd5.toString() << " módulo: " << dd5.mod() << endl;
    cout << "Punto más dirección (dd1 + dd2):     " << dd6.toString() << " módulo: " << dd6.mod() << endl;
    cout << endl;
    cout << "Dirección menos dirección (dd2 - dd3): " << dd7.toString() << " módulo: " << dd7.mod() << endl;
    cout << "Punto menos punto (dd1 - dd1):     " << dd8.toString() << " módulo: " << dd8.mod() << endl;
    cout << "Punto menos dirección (dd1 + dd2):     " << dd9.toString() << " módulo: " << dd9.mod() << endl << endl;

    float prod1 = dotProduct(dd1,dd2);

    // vect, vect2 y dd1 son perpendiculares entre sí
    DotDir vect = crossProduct(dd1,dd2);
    DotDir vect2 = crossProduct(dd1,vect);
    cout << "Producto escalar de dd1 y dd2:    " << prod1 << endl;
    cout << "Producto vectorial de dd1 y dd2:  " << vect.toString() << endl;
    cout << "Producto vectorial de dd1 y vect: " << vect2.toString() << endl;
    cout << "Producto escalar de dd1 y vect:   " << dotProduct(dd1,vect) << endl;
    cout << "Producto escalar de dd1 y vect2:  " << dotProduct(dd1,vect2) << endl;
    cout << "Producto escalar de vect y vect2: " << dotProduct(vect,vect2) << endl << endl;

    DotDir norm1 = normalization(dd1);
    DotDir norm2 = normalization(dd2);
    DotDir norm3 = normalization(dd3);
    cout << "dd1 normalizado:    " << norm1.toString() << " módulo: " << norm1.mod() << endl;
    cout << "dd2 normalizado:    " << norm2.toString() << " módulo: " << norm2.mod() << endl;
    cout << "dd3 normalizado:    " << norm3.toString() << " módulo: " << norm3.mod() << endl << endl;

    DotDir eje(0,2,0,0);
    DotDir centro(0,0,0,1);
    DotDir ref(1,0,0,1);

    cout << "Check del radio incorrecto: " << checkRadius(dd1, dd2, dd3) << endl;
    cout << "Check del radio correcto:   " << checkRadius(eje, centro, ref) << endl << endl;
    Sphere esf(centro, eje, ref);

    DotDir base[3];
    esf.getBase(base);

    cout << "Base obtenida con getBase, los vectores debe ser perpendiculares: " << endl;
    cout << "Vector 1: " << base[0].toString() << endl;
    cout << "Vector 2: " << base[1].toString() << endl;
    cout << "Vector 3: " << base[2].toString() << endl;
    cout << "Producto escalar entre 1 y 2: " << dotProduct(base[0],base[1]) << endl;
    cout << "Producto escalar entre 1 y 3: " << dotProduct(base[0],base[2]) << endl;
    cout << "Producto escalar entre 2 y 3: " << dotProduct(base[1],base[2]) << endl << endl;

    Transformation t1;
    Transformation t2;
    Transformation t3;
    Transformation inversaT1;
    Transformation inversaT2;

    cout << "Identidad:   " << endl << t1.toString() << endl << endl;
    t1.setZero();
    cout << "Zeros:       " << endl << t1.toString() << endl << endl;
    t1.identidad();
    cout << "Identidad:   " << endl << t1.toString() << endl << endl;
    t1.translation(4,5,6);
    cout << "Traslacion:  " << endl << t1.toString() << endl << endl;
    inversaT1 = inverse(t1);
    cout << "Inversa:  " << endl << inversaT1.toString() << endl << endl;
    t2.scale(1,2,3);
    cout << "Escala:      " << endl << t2.toString() << endl << endl;
    t3 = t1*t2;
    cout << "t1 * t2:     " << endl << t3.toString() << endl << endl;
    t3 = t2*t1;
    cout << "t2 * t1:     " << endl << t3.toString() << endl << endl;
    t3 = t1*inversaT1;
    cout << "t1*inv(t1):  " << endl << t3.toString() << endl << endl;
    t3 = transpuesta(t2);
    cout << "t2':         " << endl << t3.toString() << endl << endl;
    t3 = transpuesta(t1);
    cout << "t1':         " << endl << t3.toString() << endl << endl;
    

    dd9 = t2*dd1;
    cout << "t2*dd1:      " << endl << dd9.toString() << endl << endl;
    t3 = transpuesta(t2);
    dd9 = t3*dd1;
    cout << "t2'*dd1:      " << endl << dd9.toString() << endl << endl;
    return 0;
}
