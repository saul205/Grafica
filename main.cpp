#include "DotDir.h"
#include <iostream>

using namespace std;

int main(){

    DotDir eje(0,2,0,0);

    DotDir centro(0,0,0,1);

    DotDir ref(1,0,0,1);

    cout << checkRadius(eje, centro, ref) << endl;

    DotDir punto;
    punto = ref - centro;

    cout << "Hola "  << punto.ToString() << endl;

    return 0;
}