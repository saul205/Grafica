#include "DotDir.h"
#include <iostream>

using namespace std;

int main(){

    DotDir eje(0,2,0,0);

    DotDir centro(0,0,0,1);

    DotDir ref(1,0,0,1);

    cout << "checkRadius: " << checkRadius(eje, centro, ref) << endl;

    DotDir punto;
    punto = ref - centro;

    cout << "ToString: " << punto.ToString() << endl;

    DotDir base[3];
    Sphere esf(centro, eje, ref);

    esf.getBase(base);

    cout << "Base obtenida con getBase: " << endl;
    cout << "     " << base[0].ToString() << endl;
    cout << "     " << base[1].ToString() << endl;
    cout << "     " << base[2].ToString() << endl << endl;

    return 0;
}
