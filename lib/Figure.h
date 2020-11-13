#ifndef FIGURE_H
#define FIGURE_H

#include "Ray.h"
#include "DotDir.h"
#include "BoundingBox.h"
#include "Image.h"
#include "Transformation.h"

struct BRDF{
    rgb kt, ks, kd;
};

class Figure {
    private:

        rgb emission;
        bool hasTexture = false;
    
    protected:

        Image textura;
        float new_max  = 1;
        const int col_res = 255;

    public:
        Figure(){}

        BRDF material;
        
        virtual bool intersects(Ray ray, float& t, DotDir& p) = 0;
        virtual DotDir getCenter() = 0;
        virtual BoundingBox getBound() = 0;
        virtual rgb getTexture(const DotDir& interseccion) = 0;
        
        virtual void transform(Transformation t){
            cout << "jaja" << endl;
        };

        void setRgb(rgb _color){
            emission = _color;
        }

        rgb getEmission(DotDir& interseccion){
            if(!hasTexture){
                return (emission / col_res) * new_max;
            } else {
                return getTexture(interseccion) / new_max;
            }
        };

        void setTexture(const Image& _textura){
            textura = _textura;
            new_max = textura.getMaximo();
            hasTexture = true;
        }

        virtual void getBase(DotDir interseccion, DotDir& base[3]) = 0;
};

#endif