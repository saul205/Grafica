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
        virtual void transform(Transformation t){ };

        // ---- SETTERS y GETTERS ----

        void setRgb(rgb _color){
            emission = _color;
        }

        rgb getEmission(const DotDir& interseccion){
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

        rgb getDifRgb(const DotDir& interseccion){
            if(hasTexture){
                return getTexture(interseccion) * material.kd;
            }
            return material.kd;
        }

        rgb getRefRgb(){
            return material.kt;
        }

        void setDifBDRF(rgb difuso){
            material.kd = difuso;
        }

        void setSpecBDRF(rgb spec){
            material.ks = spec;
        }

        void setRefBDRF(rgb ref){
            material.kt = ref;
        }

        void setDielectrico(){
            material.dielectrico = true;
        }

};

// Obtiene las probabilidades correspondientes al comportamiento del material
void getMaterialProbabilities(shared_ptr<Figure>& f, DotDir dirrayo, DotDir normal, float& pk, float& ps, float& pt, bool bounce){

    if (f->material.isDielectrico()){

        float r = fresnel(dirrayo, normal, f->refractionIndex);
        float t = 1.f - r;

        ps = bounce ? 0.9 * r : r;
        pt = bounce ? 0.9 * t : t;
        pk = 0.f;
        f->material.kd = rgb(0., 0., 0.);
        f->material.ks = rgb(r, r, r);
        f->material.kt = rgb(t, t, t);
    }
    else{
        pk = f->material.kd.maximo();
        ps = f->material.ks.maximo();
        pt = f->material.kt.maximo();
        float maxes = pk + ps + pt;
        pk = bounce ? 0.9 * (pk / maxes) : (pk / maxes);
        ps = bounce ? 0.9 * (ps / maxes) : (ps / maxes);
        pt = bounce ? 0.9 * (pt / maxes) : (pt / maxes);
    }
}

#endif