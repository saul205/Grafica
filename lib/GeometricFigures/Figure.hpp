#ifndef FIGURE_H
#define FIGURE_H

#include "Ray.hpp"
#include "DotDir.hpp"
#include "BoundingBox.hpp"
#include "Image.hpp"
#include "Transformation.hpp"

struct BRDF{
    rgb kt,ks,kd;

    BRDF();
};

class Figure {
    private:

        rgb emission;
        bool tieneEmission = false;
        bool hasTexture = false;
    
    protected:

        Image textura;

    public:

        BRDF material;
        float refractionIndex = 1.000293;
        Figure(){}
        
        virtual bool intersects(Ray ray, float& t, DotDir& p) = 0;
        virtual DotDir getCenter() = 0;
        virtual BoundingBox getBound() = 0;
        virtual rgb getTexture(const DotDir& interseccion) = 0;
        virtual void getBase(const DotDir interseccion, DotDir& base0, DotDir& base1, DotDir& base2) = 0;
        virtual void transform(Transformation t){};

        // ---- SETTERS y GETTERS ----

        void setRgb(rgb _color);

        rgb getEmission(DotDir& interseccion);

        void setTexture(const Image& _textura);

        void setEmission(bool emi);

        bool hasEmission();

        rgb getSpecRgb();

        rgb getDifRgb();

        rgb getRefRgb();

        void setDifBDRF(rgb difuso);

        void setSepcBDRF(rgb spec);

        void setRefBDRF(rgb ref);

};

#endif