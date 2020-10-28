#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include "Figure.h"

class BoundingVolume{
    class Nodo{
        std::shared_pointer<Figure> figura;

        std::shared_pointer<Figure> hijoI;
        std::shared_pointer<Figure> hijoD;
    }
}

#endif