#include "Figure.hpp"

BRDF::BRDF()
{
    kd.set(0, 0, 0);
    ks.set(0, 0, 0);
    kt.set(0, 0, 0);
}

void Figure::setRgb(rgb _color)
{
    emission = _color;
}

rgb Figure::getEmission(DotDir &interseccion)
{
    if (!hasTexture)
    {
        return emission;
    }
    else
    {
        return getTexture(interseccion);
    }
};

void Figure::setTexture(const Image &_textura)
{
    textura = _textura;
    hasTexture = true;
}

void Figure::setEmission(bool emi)
{
    tieneEmission = emi;
}

bool Figure::hasEmission()
{
    return tieneEmission;
}

rgb Figure::getSpecRgb()
{
    return material.ks;
}

rgb Figure::getDifRgb()
{
    return material.kd;
}

rgb Figure::getRefRgb()
{
    return material.kt;
}

void Figure::setDifBDRF(rgb difuso)
{
    material.kd = difuso;
}

void Figure::setSepcBDRF(rgb spec)
{
    material.ks = spec;
}

void Figure::setRefBDRF(rgb ref)
{
    material.kt = ref;
}
