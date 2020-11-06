#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include "Figure.h"
#include <vector>

class BoundingVolume{
    struct PrimitiveInfo{

        PrimitiveInfo(){}

        PrimitiveInfo(int index, const BoundingBox &_bound){
            primitiveIndex = index;
            bound = _bound;
            centroid = bound.getCenter();
        }

        int primitiveIndex;
        BoundingBox bound;
        DotDir centroid;
    };

    struct Node{

        void InitLeaf(int first, int n, const BoundingBox& b){
            bounds = b;
            firstPrimOffset = first;
            nPrimitives = n;
            left = nullptr;
            right = nullptr;
        }

        void InitInterior(int axis, std::shared_ptr<Node> _left, std::shared_ptr<Node> _right){
            left = _left;
            right = _right;
            splitAxis = axis;
            bounds = Union(_left->bounds, _right->bounds);
            nPrimitives = 0;
        }

        BoundingBox bounds;
        int splitAxis, firstPrimOffset, nPrimitives;
        std::shared_ptr<Node> left, right;
    };

    struct LinearNode{
        BoundingBox bound;
        int nPrimitives, axis, primitivesOffset, secondChildOffset;
    };

    public:

        bool intersect(Ray ray, std::shared_ptr<Figure>& figura) const {
            bool intersect = false;
            std::vector<int> nodosAVisitar;
            nodosAVisitar.push_back(0);
            float minT = INFINITY, minTObject = -1, newT = INFINITY;
            while(nodosAVisitar.size() > 0){
                int nodeIndex = nodosAVisitar.back();
                nodosAVisitar.pop_back();

                if(nodos[nodeIndex].bound.intersects(ray, newT)){

                    if(nodos[nodeIndex].nPrimitives > 0){
                        DotDir point;
                        for(int i = 0; i < nodos[nodeIndex].nPrimitives; i++){
                            if(figuras[nodos[nodeIndex].primitivesOffset + i]->getBound().intersects(ray, newT)){
                                
                                if(newT < minT){
                                    minT = newT;
                                    minTObject = nodos[nodeIndex].primitivesOffset + i;
                                }
                                intersect = true;
                            }
                        }
                    }
                    else{
                        //cout << "Soy " << nodeIndex << " añado " << nodeIndex + 1 << " | " << nodos[nodeIndex].secondChildOffset << endl;
                        nodosAVisitar.push_back(nodos[nodeIndex].secondChildOffset);
                        nodosAVisitar.push_back(nodeIndex + 1);
                    }
                }
            }

            if(minTObject >= 0){
                figura = figuras[minTObject];
            }

            return intersect;
        }

        BoundingVolume(std::vector<std::shared_ptr<Figure>> figuras) : figuras(figuras){
            if(figuras.empty()){
                return;
            }

            std::vector<PrimitiveInfo> primitiveInfo(figuras.size());
            for(int i = 0; i < figuras.size(); i ++){
                primitiveInfo[i] = {i, figuras[i]->getBound()};
            }

            int totalNodes = 0;
            std::vector<std::shared_ptr<Figure>> orderedFigures;
            orderedFigures.reserve(figuras.size());

            std::shared_ptr<Node> root(recursiveBuild(primitiveInfo, 0, figuras.size(), totalNodes, orderedFigures));

            figuras.swap(orderedFigures);
            primitiveInfo.clear();

            nodos.reserve(totalNodes);
            int offset = 0;
            int offset2 = flattenTree(root, offset);

            int i = 0;
            for(LinearNode l : nodos){
                cout << i << ": " << l.primitivesOffset << " | " << l.secondChildOffset << endl;
                if(l.nPrimitives > 0){
                    cout << figuras[l.primitivesOffset]->getEmission().r << " " << figuras[l.primitivesOffset]->getEmission().g << " " << figuras[l.primitivesOffset]->getEmission().b << endl;
                }
                //cout << l.bound.getTop().toString() << endl;
                //cout << l.bound.getBottom().toString() << endl;
                cout << figuras[l.primitivesOffset]->getCenter().toString() << endl;
                i++;
            }
        }

        std::shared_ptr<Node> recursiveBuild(std::vector<PrimitiveInfo> &primitiveInfo, int start, int end, int &totalNodes, 
                                             std::vector<std::shared_ptr<Figure>>& orderedFigures){
            std::shared_ptr<Node> nodo(new Node()); 
            int nPrimitives = end - start;
            totalNodes ++;

            BoundingBox bounds;
            for(int i = start; i < end; i++){
                bounds = Union(bounds, primitiveInfo[i].bound);
            }

            if(nPrimitives == 1){
                int firstPrimOffset = orderedFigures.size();
                for(int i = start; i < end; i ++){
                    orderedFigures.push_back(figuras[primitiveInfo[i].primitiveIndex]);
                }
                nodo->InitLeaf(firstPrimOffset, nPrimitives, bounds);
                return nodo;
            }else{
                BoundingBox centroidBounds;
                for(int i = start; i < end; i++){
                    centroidBounds = Union(centroidBounds, primitiveInfo[i].bound);
                }
                int dim = centroidBounds.MaxAxe();
                int mid = (start + end) / 2;

                if(centroidBounds.getTop()[dim] == centroidBounds.getBottom()[dim]){
                    int firstPrimOffset = orderedFigures.size();
                    for(int i = start; i < end; i ++){
                        orderedFigures.push_back(figuras[primitiveInfo[i].primitiveIndex]);
                    }
                    nodo->InitLeaf(firstPrimOffset, nPrimitives, bounds);
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
                    recursiveBuild(primitiveInfo, start, mid, totalNodes, orderedFigures),
                    recursiveBuild(primitiveInfo, mid, end, totalNodes, orderedFigures));
            }
            return nodo;
        }
       
    private:

        std::shared_ptr<Node> root;
        std::vector<std::shared_ptr<Figure>> figuras;
        std::vector<LinearNode> nodos;

        int flattenTree(std::shared_ptr<Node> nodo, int& offset){
            nodos.push_back(LinearNode());
            int myOffset = offset++;
            nodos[myOffset].bound = nodo->bounds;
            
            if(nodo->nPrimitives > 0){
                nodos[myOffset].primitivesOffset = nodo->firstPrimOffset;
                nodos[myOffset].nPrimitives = nodo->nPrimitives;
            }else{
                nodos[myOffset].axis = nodo->splitAxis;
                nodos[myOffset].nPrimitives = 0;
                flattenTree(nodo->left, offset);
                nodos[myOffset].secondChildOffset = flattenTree(nodo->right, offset);
            }
            return myOffset;
        }

};

#endif