#ifndef SENSOR_H
#define SENSOR_H

#include "DotDir.h"
#include "Transformation.h"

class Sensor{
    private:

        DotDir f, i, u;
        DotDir center;

    public:

        Sensor(float distance, float up, float left){
            f.setDotDir(0, 0, distance, 0);
            i.setDotDir(left, 0, 0, 0);
            u.setDotDir(0, up, 0, 0);

            center.setDotDir(0, 0, 0, 1);
        }

};

#endif