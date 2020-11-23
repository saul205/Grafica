#include "Scene.h"

// Recibe tamaño de la imagen + target y front vector
Scene::Scene(float width, float height, DotDir target, DotDir front, float col_res)
{
    imagen = Image("P3", "IMG", width, height, col_res, col_res);
    renderer = Sensor(30.0f, width / height, target, width, height, front);
    color_res = col_res;
}

int Scene::addPlane(DotDir vHeight, DotDir vWidth, DotDir center, float height, float width, rgb color, bool em)
{
    std::shared_ptr<Figure> plano(new Plane(
        vHeight,
        vWidth,
        center,
        height,
        width));
    plano->setEmission(em);
    plano->setRgb(color);
    figuras.push_back(plano);
    return figuras.size() - 1;
}

int Scene::addPlane(DotDir vHeight, DotDir vWidth, DotDir center, float height, float width, const Image &textura)
{
    std::shared_ptr<Figure> plano(new Plane(
        vHeight,
        vWidth,
        center,
        height,
        width));

    plano->setTexture(textura);
    figuras.push_back(plano);
    return figuras.size() - 1;
}

int Scene::addPlane(std::shared_ptr<Figure> plano)
{
    figuras.push_back(plano);
    return figuras.size() - 1;
}

int Scene::addSphere(DotDir c, DotDir axe, DotDir reference, rgb color)
{
    if (checkRadius(axe, c, reference))
    {
        std::shared_ptr<Figure> esfera(new Sphere(c, axe, reference));
        esfera->setRgb(color);
        figuras.push_back(esfera);
        return figuras.size() - 1;
    }
    else
    {
        cout << "Error en la esfera." << endl;
        return -1;
    }
}

int Scene::addSphere(DotDir c, DotDir axe, DotDir reference, const Image &textura, bool em)
{
    if (checkRadius(axe, c, reference))
    {
        std::shared_ptr<Figure> esfera(new Sphere(c, axe, reference));
        esfera->setTexture(textura);
        esfera->setEmission(em);
        figuras.push_back(esfera);
        return figuras.size() - 1;
    }
    else
    {
        cout << "Error en la esfera." << endl;
        return -1;
    }
}

int Scene::addTriangle(DotDir v1, DotDir v2, DotDir v3, rgb color)
{
    std::shared_ptr<Figure> triangle(new Triangle(v1, v2, v3));
    triangle->setRgb(color);
    figuras.push_back(triangle);

    return figuras.size() - 1;
}

int Scene::addTriangle(DotDir v1, DotDir v2, DotDir v3, const Image &textura)
{
    std::shared_ptr<Figure> triangle(new Triangle(v1, v2, v3));
    triangle->setTexture(textura);
    figuras.push_back(triangle);
    return figuras.size() - 1;
}

int Scene::addTriangle(std::shared_ptr<Figure> triangle)
{
    figuras.push_back(triangle);
    return figuras.size() - 1;
}

int Scene::addLight(DotDir position, rgb emision)
{
    luces.push_back(LightSource(emision, position));
    return luces.size() - 1;
}

void Scene::render(string output, int AA, int hilos, int mode)
{
    BoundingVolume bv(figuras);
    cout << "N Figuras: " << figuras.size() << endl;
    cout << "N Nodos: " << bv.getSize() << endl;
    renderer.lanzarRayos(bv, luces, imagen, AA, hilos);

    ToneMapper tm;
    tm.gammaCurveAndClamping(imagen, imagen.getMaximo() / 20.0f, 1 / 4.f);

    if (mode == 1)
    {
        escribir(output + ".ppm", imagen, color_res);
    }
    else
    {
        escribirbmp(output + ".bmp", imagen, color_res);
    }
}

void Scene::addTriangleMesh(TriangleMesh t)
{
    int size = t.getSize();
    for (float i = 0; i < size; i++)
    {
        Triangle tri = t[i];
        tri.setDifBDRF(rgb(0.3, 0, 0.3));
        tri.setSepcBDRF(rgb(0.2, 0, 0.2));
        //tri.setRgb(rgb(255, 255, 255));
        figuras.push_back(std::make_shared<Triangle>(tri));
    }
}

std::shared_ptr<Figure> Scene::getFigure(int n)
{
    return figuras[n];
}
