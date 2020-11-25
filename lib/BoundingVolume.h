#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include "Figure.h"
#include <vector>
#include <algorithm>

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

        PrimitiveInfo(int index, const BoundingBox &_bound){
            primitiveIndex = index;
            bound = _bound;
            centroid = bound.getCenter();
            //cout << "Centroid " << centroid.toString() << endl;
        }

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

        void InitLeaf(const BoundingBox& b){
            bounds = b;
            left = nullptr;
            right = nullptr;
        }

        void addPrimitive(int n){
            primitives.push_back(n);
        }

        void InitInterior(int axis, std::shared_ptr<Node> _left, std::shared_ptr<Node> _right){
            left = _left;
            right = _right;
            splitAxis = axis;
            bounds = Union(_left->bounds, _right->bounds);
        }

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
        bool intersect(Ray ray, std::shared_ptr<Figure>& figura, DotDir& interseccion, float &intersecciones) const {
            bool intersect = false;
            std::vector<int> nodosAVisitar;
            nodosAVisitar.push_back(0);
            float minT = INFINITY, minTObject = -1, newT = INFINITY;
            while(nodosAVisitar.size() > 0){
                int nodeIndex = nodosAVisitar.back();
                nodosAVisitar.pop_back();
                newT = INFINITY;
                intersecciones ++;
                bool intersecta = nodos[nodeIndex].bound.intersects(ray, newT);

                if(intersecta && newT < minT){
                    if(nodos[nodeIndex].primitives.size() > 0){
                        DotDir point;
                        newT = INFINITY;
                        for(int fig : nodos[nodeIndex].primitives){
                            intersecciones ++;
                            if(figuras[fig]->intersects(ray, newT, point)){
                                
                                if(newT < minT){
                                    minT = newT;
                                    minTObject = fig;
                                    interseccion = point;
                                }
                                intersect = true;
                            }
                        }
                    }
                    else{
                        nodosAVisitar.push_back(nodos[nodeIndex].secondChildOffset);
                        nodosAVisitar.push_back(nodos[nodeIndex].firstChildOffset);
                    }
                }
            }

            if(minTObject >= 0){
                figura = figuras[minTObject];
            }
            return intersect;
        }

        // Construye la bounding volume hierarchy
        BoundingVolume(std::vector<std::shared_ptr<Figure>> figuras) : figuras(figuras){
            if(figuras.empty()){
                return;
            }

            std::vector<PrimitiveInfo> primitiveInfo(figuras.size());
            for(int i = 0; i < figuras.size(); i ++){
                primitiveInfo[i] = {i, figuras[i]->getBound()};
            }

            int totalNodes = 0;

            std::shared_ptr<Node> root(recursiveBuild(primitiveInfo, 0, figuras.size(), totalNodes));

            primitiveInfo.clear();

            nodos.reserve(totalNodes);
            int offset = 0;
            int offset2 = flattenTree(root, offset);
        }

        // Construye el árbol de la BVH y devuelve la raíz del mismo
        std::shared_ptr<Node> recursiveBuild(std::vector<PrimitiveInfo> &primitiveInfo, int start, int end, int &totalNodes){
            std::shared_ptr<Node> nodo(new Node()); 
            int nPrimitives = end - start;
            totalNodes ++;

            BoundingBox bounds;
            for(int i = start; i < end; i++){
                bounds = Union(bounds, primitiveInfo[i].bound);
            }

            //cout << start << " -> " << end << endl << bounds.getTop().toString() << endl << bounds.getBottom().toString() << endl;

            if(nPrimitives == 1){
                nodo->InitLeaf(bounds);
                for(int i = start; i < end; i ++){
                    nodo->primitives.push_back(primitiveInfo[i].primitiveIndex);
                }
                return nodo;
            }else{
                BoundingBox centroidBounds;
                for(int i = start; i < end; i++){
                    centroidBounds = Union(centroidBounds, primitiveInfo[i].centroid);
                }
                int dim = centroidBounds.MaxAxe();
                int mid = (start + end) / 2;

                if(centroidBounds.getTop()[dim] == centroidBounds.getBottom()[dim]){
                    nodo->InitLeaf(bounds);
                    for(int i = start; i < end; i ++){
                        nodo->primitives.push_back(primitiveInfo[i].primitiveIndex);
                    }
                    return nodo;
                }else{
                    //Primera prueba con Centros
                    
                    float pMid = (centroidBounds.getTop()[dim] + centroidBounds.getBottom()[dim]) / 2;
                    PrimitiveInfo *midPtr = std::partition(&primitiveInfo[start], &primitiveInfo[end-1]+1, 
                            [dim, pMid](const PrimitiveInfo &p){
                                return p.centroid[dim] < pMid;
                            });

                    mid = midPtr - &primitiveInfo[0];
                    if(mid == start || mid == end){
                        mid = (start + end) / 2;
                        std::nth_element(&primitiveInfo[start], &primitiveInfo[mid],
                            &primitiveInfo[end - 1] + 1,
                            [dim](const PrimitiveInfo& a, const PrimitiveInfo& b){
                                return a.centroid[dim] < b.centroid[dim];
                            });
                    }
                }

                nodo->InitInterior(dim, 
                    recursiveBuild(primitiveInfo, start, mid, totalNodes),
                    recursiveBuild(primitiveInfo, mid, end, totalNodes));
            }
            return nodo;
        }

        float getSize(){
            return nodos.size();
        }
       
    private:

        std::shared_ptr<Node> root;
        std::vector<std::shared_ptr<Figure>> figuras;
        std::vector<LinearNode> nodos;

        int flattenTree(std::shared_ptr<Node> nodo, int& offset){
            nodos.push_back(LinearNode());
            int myOffset = offset++;
            nodos[myOffset].bound = nodo->bounds;
            nodos[myOffset].primitives = nodo->primitives;
            
            if(nodo->primitives.size() > 0){
            }else{
                nodos[myOffset].axis = nodo->splitAxis;
                nodos[myOffset].firstChildOffset = flattenTree(nodo->left, offset);
                nodos[myOffset].secondChildOffset = flattenTree(nodo->right, offset);
            }
            return myOffset;
        }

};

#endif