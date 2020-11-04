#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "DotDir.h"
#include <float.h>

class BoundingBox {

    private:

        DotDir top, bottom;

    public:

        friend BoundingBox Union(const BoundingBox& box1, const BoundingBox& box2);

        BoundingBox(){
            bottom.setDotDir(FLT_MAX, FLT_MAX, FLT_MAX, 1);
            top.setDotDir(-FLT_MAX, -FLT_MAX, -FLT_MAX, 1);
        }

        BoundingBox(DotDir _top, DotDir _bottom) {
            top.setDotDir(std::max(_top.getX(), _bottom.getX()),
                          std::max(_top.getY(), _bottom.getY()),
                          std::max(_top.getZ(), _bottom.getZ()),
                          1 );

            bottom.setDotDir(std::min(_top.getX(), _bottom.getX()),
                             std::min(_top.getY(), _bottom.getY()),
                             std::min(_top.getZ(), _bottom.getZ()),
                             1 );
        }

        bool intersects(Ray ray) {
            DotDir r = ray.getDir();
            return (r.getX() >= bottom.getX() && r.getX() <= top.getX() &&
                    r.getY() >= bottom.getY() && r.getY() <= top.getY() &&
                    r.getZ() >= bottom.getZ() && r.getZ() <= top.getZ());
        }

        bool intersects(Ray ray) const {
            float tmin = -INFINITY, tmax = INFINITY;

            if(ray.getDir().getX() != 0){
                float tx1 = (bottom.getX() - ray.getOrigen().getX()) / ray.getDir().getX();
                float tx2 = (top.getX() - ray.getOrigen().getX()) / ray.getDir().getX();

                tmin = max(tmin, min(tx1,tx2));
                tmax = min(tmax, max(tx1,tx2));
            }

            if(ray.getDir().getY() != 0){
                float ty1 = (bottom.getY() - ray.getOrigen().getY()) / ray.getDir().getY();
                float ty2 = (top.getY() - ray.getOrigen().getY()) / ray.getDir().getY();

                tmin = max(tmin, min(ty1,ty2));
                tmax = min(tmax, max(ty1,ty2));
            }

            return tmax >= tmin;
        }

        DotDir getCenter() {
            return DotDir(
                bottom.getX() + 0.5f * (top.getX() - bottom.getX()),
                bottom.getY() + 0.5f * (top.getY() - bottom.getY()),
                bottom.getZ() + 0.5f * (top.getZ() - bottom.getZ()),
                1
                );
        }

        DotDir getTop(){
            return top;
        }

        DotDir getBottom(){
            return bottom;
        }

        int MaxAxe(){
            float x = top.getX() - bottom.getX();
            float y = top.getY() - bottom.getY();
            float z = top.getZ() - bottom.getZ();
            if(x > y && x > z){
                return 0;
            }else if(y > z){
                return 1;
            }else{
                return 2;
            }
        }

};


BoundingBox Union(const BoundingBox& box1, const BoundingBox& box2){
    return BoundingBox(DotDir(std::max(box1.top.getX(), box2.top.getX()),
                                std::max(box1.top.getY(), box2.top.getY()),
                                std::max(box1.top.getZ(), box2.top.getZ()),
                                1), 

                        DotDir(std::min(box1.bottom.getX(), box2.bottom.getX()),
                                std::min(box1.bottom.getY(), box2.bottom.getY()),
                                std::min(box1.bottom.getZ(), box2.bottom.getZ()),
                                1));
}


#endif