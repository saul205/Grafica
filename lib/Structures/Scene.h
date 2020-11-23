#ifndef SCENE_H
#define SCENE_H

#include "BoundingVolume.h"
#include "Sensor.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "ToneMapper.h"
#include "TriangleMesh.h"

class Scene {
    private:

        Image imagen;
        Sensor renderer;    
        float color_res;    // Color resolution

        std::vector<shared_ptr<Figure>> figuras;
        std::vector<LightSource> luces;

    public:


        // Recibe tamaño de la imagen + target y front vector
        Scene(float width, float height, DotDir target, DotDir front = DotDir(0,0,1,0), float col_res = 255);

        // Añade un plano a la imagen definido por vHeight, wWidth, center, height, width y color
        int addPlane(DotDir vHeight, DotDir vWidth, DotDir center, float height, float width, rgb color, bool em = false);

        // Añade un plano a la imagen definido por vHeight, wWidth, center, height, width y con textura textura
        int addPlane(DotDir vHeight, DotDir vWidth, DotDir center, float height, float width, const Image& textura);

        // Añade el plano a la imagen
        int addPlane(std::shared_ptr<Figure> plano);

        // Añade la esfera definida por el centro c, eje axe y ciudad de referencia reference. Le da el color color.
        int addSphere(DotDir c, DotDir axe, DotDir reference, rgb color);

        // Añade la esfera definida por el centro c, eje axe y ciudad de referencia reference. Le da la textura textura.
        int addSphere(DotDir c, DotDir axe, DotDir reference, const Image& textura, bool em = false);

        // Añade a la imagen el triángulo definido por v1, v2 y v3, le da el color color
        int addTriangle(DotDir v1, DotDir v2, DotDir v3, rgb color);

        // Añade a la imagen el triángulo definido por v1, v2 y v3, le aplica la textura textura
        int addTriangle(DotDir v1, DotDir v2, DotDir v3, const Image& textura);

        // Añade triangle a la imagen
        int addTriangle(std::shared_ptr<Figure> triangle);

        // Añade una luz puntual en position y con la emisión emision a la imagen
        int addLight(DotDir position, rgb emision);

        // Crea la BVH, lleva a cabo el pathtracing y escribe la imagen correspondiente
        // a la escena. Utiliza para ello hilos threads, y lanza AA rayos por píxel.
        // Si mode = 1 escribe la imagen en formato .ppm, sino en .bmp
        void render(string output, int AA, int hilos = 1, int mode = 1);

        // Añade la malla de triángulos t a la escena
        void addTriangleMesh(TriangleMesh t);

        // Devuelve la figura de la escena correspondiente al índice n
        std::shared_ptr<Figure> getFigure(int n);

};

#endif