#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include "Figure.hpp"
#include <vector>
#include <algorithm>
#include <memory>

class BoundingVolume{

    /*
     * Es el nodo de la la lista que tiene la infromación de una
     * figura precalculada, su índice (a qué figura de la lista de
     * figuras representa), su bounding box y su centroide.
     * Se usa para tener los datos precalculados en lugar de tener
     * que calcularlos constantemente.
     */
    struct PrimitiveInfo{

        PrimitiveInfo(){}

        PrimitiveInfo(int index, const BoundingBox &_bound);

        int primitiveIndex;
        BoundingBox bound;
        DotDir centroid;
    };

    /*
     * Nodo del árbol en construcción, con hijos izquierdo y derecho,
     * lista de primitivas que contiene, boundingBox y eje por el que se parte
     * la bounding box en la división en hijos.
     */
    struct Node{

        void InitLeaf(const BoundingBox& b);

        void addPrimitive(int n);

        void InitInterior(int axis, std::shared_ptr<Node> _left, std::shared_ptr<Node> _right);

        std::vector<int> primitives;
        BoundingBox bounds;
        int splitAxis;
        std::shared_ptr<Node> left, right;
    };

    /*
     * Nodo del árbol aplanado, contiene la bounding box, los índices de las figuras
     * que contiene, el axis por el que se parte para llegar a sus hijosy los índices
     * de su hijo izquierdo y derecho.
     */
    struct LinearNode{
        BoundingBox bound;
        std::vector<int> primitives;
        int axis, firstChildOffset, secondChildOffset;
    };

    public:

        // Devuelve cierto si y solo si el rayo ray intersecta con alguna figura, de ser así, 
        // el valor de interseccion es el del punto de interseccion
        bool intersect(Ray ray, std::shared_ptr<Figure>& figura, DotDir& interseccion, float &intersecciones) const;

        // Construye la bounding volume hierarchy
        BoundingVolume(std::vector<std::shared_ptr<Figure>> figuras);

        // Construye el árbol de la BVH y devuelve la raíz del mismo
        std::shared_ptr<Node> recursiveBuild(std::vector<PrimitiveInfo> &primitiveInfo, int start, int end, int &totalNodes);

        // Devuelve el número de nodos del árbol
        int getSize();
       
    private:

        std::shared_ptr<Node> root;
        std::vector<std::shared_ptr<Figure>> figuras;
        std::vector<LinearNode> nodos;

        // Aplana el árbol
        int flattenTree(std::shared_ptr<Node> nodo, int& offset);

};

#endif