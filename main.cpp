#include "DotDir.h"
#include <iostream>

using namespace std;

int main(){

    DotDir punto(1,1,1,1);
    DotDir punto2(2, 2, 2, 2);

    punto = punto + punto2;

    cout << "Hola "  << punto.ToString() << endl;

    return 0;
}