#ifndef FIGURE_H
#define FIGURE_H

#include "Ray.h"
#include "DotDir.h"
#include "BoundingBox.h"
#include "Image.h"
#include "Transformation.h"

struct BRDF{
    rgb kt,ks,kd;

    bool dielectrico = false;

    BRDF(bool di = false){
        kd.set(0,0,0);
        ks.set(0,0,0);
        kt.set(0,0,0);

        dielectrico = di;
    }

    bool isDielectrico(){
        return dielectrico;
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

    public:

        BRDF material;
        float refractionIndex = 1.000293;
        Figure(){}
        
        virtual bool intersects(Ray ray, float& t, DotDir& p) = 0;
        virtual DotDir getCenter() = 0;
        virtual BoundingBox getBound() = 0;
        virtual rgb getTexture(const DotDir& interseccion) = 0;
        virtual void getBase(const DotDir interseccion, DotDir& base0, DotDir& base1, DotDir& base2) = 0;
        virtual void transform(Transformation t){ cout << "jaja" << endl; };

        // ---- SETTERS y GETTERS ----

        void setRgb(rgb _color){
            emission = _color;
        }

        rgb getEmission(DotDir& interseccion){
            if(!hasTexture){
                return emission;
            } else {
                return getTexture(interseccion);
            }
        };

        void setTexture(const Image& _textura){
            textura = _textura;
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

        rgb getRefRgb(){
            return material.kt;
        }

        void setDifBDRF(rgb difuso){
            material.kd = difuso;
        }

        void setSepcBDRF(rgb spec){
            material.ks = spec;
        }

        void setRefBDRF(rgb k){
            material.ks = k;
            material.dielectrico = true;
        }

};

#endif