#ifndef FIGURE_H
#define FIGURE_H

#include "Ray.h"
#include "DotDir.h"
#include "BoundingBox.h"
#include "Image.h"
#include "Transformation.h"

struct BRDF{
    rgb kt,ks,kd;

    BRDF(){
        kd.set(0,0,0);
        ks.set(0.7,0.7,0.7);
        kt.set(0,0,0);
    }
};

class Figure {
    private:

        rgb emission;
        bool tieneEmission = false;
        bool hasTexture = false;
    
    protected:

        Image textura;
        float new_max  = 1;
        int col_res = 255;

    public:

        BRDF material;
        Figure(){}
        
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
            /*if(!hasTexture){
                return (emission / col_res) * new_max;
            } else {
                return getTexture(interseccion) / new_max;
            }*/
            if(!hasTexture){
                return emission;
            } else {
                return getTexture(interseccion);
            }
        };

        void setTexture(const Image& _textura){
            textura = _textura;
            new_max = textura.getMaximo();
            hasTexture = true;
        }

        void setEmission(bool emi) {
             tieneEmission = emi;
        }

        bool hasEmission(){
            return tieneEmission;
        }

        rgb getSpecRgb(){
            return material.ks;
        }

        rgb getDifRgb(){
            return material.kd;
        }

        virtual void getBase(DotDir interseccion, DotDir base[3]) = 0;
};

#endif