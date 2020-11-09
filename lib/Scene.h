#ifndef SCENE_H
#define SCENE_H

#include "../lib/Sensor.h"
#include "../lib/Plane.h"
#include "../lib/Sphere.h"
#include "../lib/Triangle.h"
#include "../lib/ToneMapper.h"

class Scene{
    private:
        Image imagen;
        Sensor renderer;
        float color_res;

        std::vector<Figure*> figuras;

    public:

        Scene(float width, float height, DotDir c1, DotDir c2, DotDir c3, DotDir c4, float col_res = 255){
            imagen = Image("P3", "IMG", width, height, col_res, col_res);
            renderer = Sensor(c1, c2, c3, c4, width, height);
            color_res = col_res;
        }
        
        ~Scene(){
            for(Figure* f : figuras){
                free(f);
            }
        }

        int addPlane(DotDir vHeight, DotDir vWidth, DotDir center, float height, float width, rgb color){
            Figure*  plano(new Plane(
                                    vHeight,
                                    vWidth, 
                                    center,
                                    height,
                                    width
                                    ));
            plano->setRgb(color);
            figuras.push_back(plano);
            return figuras.size() - 1;
        }

        int addPlane(DotDir vHeight, DotDir vWidth, DotDir center, float height, float width, const Image& textura){
            Figure*  plano = new Plane(
                                    vHeight,
                                    vWidth, 
                                    center,
                                    height,
                                    width
                                    );
            plano->setTexture(textura);
            figuras.push_back(plano);
            return figuras.size() - 1;
        }

        int addSphere(DotDir c, DotDir axe, DotDir reference, rgb color){
            if(checkRadius(axe, c, reference) ){
                Figure*  esfera = new Sphere(c, axe, reference);
                esfera->setRgb(color);
                figuras.push_back(esfera);

                return figuras.size() - 1;
            } else {
                cout << "Error en la esfera." << endl;
                return -1;
            }
        }

        int addSphere(DotDir c, DotDir axe, DotDir reference, const Image& textura){
            if(checkRadius(c, axe, reference) ){
                Figure*  esfera = new Sphere(c, axe, reference);
                esfera->setTexture(textura);
                figuras.push_back(esfera);

                return figuras.size() - 1;
            } else {
                cout << "Error en la esfera." << endl;
                return -1;
            }
        }

        int addTriangle(DotDir v1, DotDir v2, DotDir v3, rgb color){
            Figure*  triangle = new Triangle(v1, v2, v3);
            triangle->setRgb(color);
            figuras.push_back(triangle);

            return figuras.size() - 1;
        }

        int addTriangle(DotDir v1, DotDir v2, DotDir v3, const Image& textura){
            Figure*  triangle = new Triangle(v1, v2, v3);
            triangle->setTexture(textura);
            figuras.push_back(triangle);

            return figuras.size() - 1;
        }

        void render(string output, int AA, int hilos = 1, int mode = 1){
            cout << "N Figuras: " << figuras.size() << endl;
            renderer.lanzarRayos(figuras, imagen, AA, hilos);
            if(mode == 1){
                escribir(output + ".ppm", imagen, color_res);
            }else{
                escribirbmp(output + ".bmp", imagen, color_res);
            }
        }
};

#endif