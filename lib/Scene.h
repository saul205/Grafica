#ifndef SCENE_H
#define SCENE_H

#include "../lib/BoundingVolume.h"
#include "../lib/Sensor.h"
#include "../lib/Plane.h"
#include "../lib/Sphere.h"
#include "../lib/Triangle.h"
#include "../lib/ToneMapper.h"
#include "TriangleMesh.h"

class Scene {
    private:
        Image imagen;
        Sensor renderer;
        float color_res;

        std::vector<shared_ptr<Figure>> figuras;
        std::vector<LightSource> luces;

    public:

       Scene(float width, float height, DotDir target, DotDir front = DotDir(0,0,1,0), float col_res = 255){
            imagen = Image("P3", "IMG", width, height, col_res, col_res);
            renderer = Sensor(30.0f, width/height, target, width, height, front);
            color_res = col_res;
        }

        int addPlane(DotDir vHeight, DotDir vWidth, DotDir center, float height, float width, rgb color, bool em = false){
            std::shared_ptr<Figure> plano(new Plane(
                                    vHeight,
                                    vWidth, 
                                    center,
                                    height,
                                    width
                                    ));
            plano->setEmission(em);
            plano->setRgb(color);
            figuras.push_back(plano);
            return figuras.size() - 1;
        }

        int addPlane(DotDir vHeight, DotDir vWidth, DotDir center, float height, float width, const Image& textura){
            std::shared_ptr<Figure> plano(new Plane(
                                    vHeight,
                                    vWidth, 
                                    center,
                                    height,
                                    width
                                    ));
                                    
            plano->setTexture(textura);
            figuras.push_back(plano);
            return figuras.size() - 1;
        }

        int addPlane(std::shared_ptr<Figure> plano){
            figuras.push_back(plano);
            return figuras.size() - 1;
        }

        int addSphere(DotDir c, DotDir axe, DotDir reference, rgb color, bool em = false){
            if(checkRadius(axe, c, reference) ){
                std::shared_ptr<Figure> esfera(new Sphere(c, axe, reference));
                esfera->setRgb(color);
                esfera->setEmission(em);
                figuras.push_back(esfera);
                return figuras.size() - 1;
            } else {
                cout << "Error en la esfera." << endl;
                return -1;
            }
        }

        int addSphere(DotDir c, DotDir axe, DotDir reference, const Image& textura){
            if(checkRadius(axe, c, reference) ){
                std::shared_ptr<Figure> esfera(new Sphere(c, axe, reference));
                esfera->setTexture(textura);
                figuras.push_back(esfera);
                return figuras.size() - 1;
            } else {
                cout << "Error en la esfera." << endl;
                return -1;
            }
        }

        int addTriangle(DotDir v1, DotDir v2, DotDir v3, rgb color){
            std::shared_ptr<Figure> triangle(new Triangle(v1, v2, v3));
            triangle->setRgb(color);
            figuras.push_back(triangle);

            return figuras.size() - 1;
        }

        int addTriangle(DotDir v1, DotDir v2, DotDir v3, const Image& textura){
            std::shared_ptr<Figure> triangle(new Triangle(v1, v2, v3));
            triangle->setTexture(textura);
            figuras.push_back(triangle);
            return figuras.size() - 1;
        }

        int addTriangle(DotDir v1, DotDir v2, DotDir v3, const Image& textura, triangleVertexUV t){
            std::shared_ptr<Figure> triangle(new Triangle(v1, v2, v3, t));
            triangle->setTexture(textura);
            figuras.push_back(triangle);
            return figuras.size() - 1;
        }

        int addTriangle(std::shared_ptr<Figure> triangle){
            figuras.push_back(triangle);
            return figuras.size() - 1;
        }

        int addLight(DotDir position, rgb emision){
            luces.push_back(LightSource(emision, position));
            return luces.size() - 1;
        }

        void render(string output, int AA, int hilos = 1, int mode = 1){
            BoundingVolume bv(figuras);
            cout << "N Figuras: " << figuras.size() << endl;
            cout << "N Nodos: " << bv.getSize() << endl;
            renderer.lanzarRayos(bv, luces, imagen, AA, hilos);

            ToneMapper tm;
            tm.gammaCurveAndClamping(imagen, imagen.getMaximo() / 50.0f, 1/4.0f);

            if(mode == 1){
                escribir(output + ".ppm", imagen, color_res);
            }else{
                escribirbmp(output + ".bmp", imagen, color_res);
            }
        }

        void addTriangleMesh(TriangleMesh t){
            int size = t.getSize();
            for(float i = 0; i < size; i++){
                Triangle tri = t[i];
                tri.setRgb(rgb(i, i, i) / size * 255);
                //tri.setRgb(rgb(255, 255, 255));
                figuras.push_back(std::make_shared<Triangle>(tri));
            }
        }

        std::shared_ptr<Figure> getFigure(int n){
            return figuras[n];
        }
};

#endif