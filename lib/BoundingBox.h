#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Figure.h"
#include "DotDir.h"

class BoundingBox : public Figure{

    private:

        DotDir top, bottom;

    public:

        BoundingBox(){
            top.setDotDir(FLT_MAX, FLT_MAX, FLT_MAX, 1);
            bottom.setDotDir(-FLT_MAX, -FLT_MAX, -FLT_MAX, 1);
        }

        BoundingBox(DotDir _top, DotDir _bottom){
            top.setDotDir(std::max(_top.getX(), _bottom.getX()),
                          std::max(_top.getY(), _bottom.getY()),
                          std::max(_top.getZ(), _bottom.getZ()) );

            bottom.setDotDir(std::min(_top.getX(), _bottom.getX()),
                             std::min(_top.getY(), _bottom.getY()),
                             std::min(_top.getZ(), _bottom.getZ()) );
        }

        bool intersects(Ray ray, float& t, DotDir& p) override {

        }

        BoundingBox Union(const BoundingBox& box1, const BoundingBox& box2){
            return BoundingBox(DotDir(std::max(box1.top.x, box2.top.x),
                                      std::max(box1.top.y, box2.top.y),
                                      std::max(box1.top.z, box2.top.z)), 

                               DotDir(std::min(box1.bottom.x, box2.bottom.x),
                                      std::min(box1.bottom.y, box2.bottom.y),
                                      std::min(box1.bottom.z, box2.bottom.z));
        }

}


#endif