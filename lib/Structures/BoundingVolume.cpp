#include "BoundingVolume.hpp"

BoundingVolume::PrimitiveInfo::PrimitiveInfo(int index, const BoundingBox &_bound)
{
    primitiveIndex = index;
    bound = _bound;
    centroid = bound.getCenter();
}

void BoundingVolume::Node::InitLeaf(const BoundingBox &b)
{
    bounds = b;
    left = nullptr;
    right = nullptr;
}

void BoundingVolume::Node::addPrimitive(int n)
{
    primitives.push_back(n);
}

void BoundingVolume::Node::InitInterior(int axis, std::shared_ptr<Node> _left, std::shared_ptr<Node> _right)
{
    left = _left;
    right = _right;
    splitAxis = axis;
    bounds = Union(_left->bounds, _right->bounds);
}

BoundingVolume::BoundingVolume(std::vector<std::shared_ptr<Figure>> figuras) : figuras(figuras)
{
    if (figuras.empty())
        return;

    // Prepara el vector de primitive info
    std::vector<PrimitiveInfo> primitiveInfo(figuras.size());
    for (int i = 0; i < figuras.size(); i++){
        primitiveInfo[i] = {i, figuras[i]->getBound()};
    }

    int totalNodes = 0;

    // Construye el árbol
    std::shared_ptr<Node> root(recursiveBuild(primitiveInfo, 0, figuras.size(), totalNodes));

    primitiveInfo.clear();

    // Aplana el árbol
    nodos.reserve(totalNodes);
    int offset = 0;
    int offset2 = flattenTree(root, offset);
}

bool BoundingVolume::intersect(Ray ray, std::shared_ptr<Figure> &figura, DotDir &interseccion, float &intersecciones) const
{

    bool intersect = false;
    std::vector<int> nodosAVisitar;
    nodosAVisitar.push_back(0);
    float minT = INFINITY, minTObject = -1, newT = INFINITY;

    // Mientras que queden posibles nodos que intersectar
    while (nodosAVisitar.size() > 0)
    {

        int nodeIndex = nodosAVisitar.back();
        nodosAVisitar.pop_back();
        newT = INFINITY;
        intersecciones++;
        bool intersecta = nodos[nodeIndex].bound.intersects(ray, newT);

        // Si intersecta más cerca
        if (intersecta && newT < minT)
        {
            // Si tiene alguna primitiva
            if (nodos[nodeIndex].primitives.size() > 0)
            {
                DotDir point;
                newT = INFINITY;
                // Para todas las primitivas del nodo
                for (int fig : nodos[nodeIndex].primitives)
                {
                    intersecciones++;
                    // Si intersecta con la figura
                    if (figuras[fig]->intersects(ray, newT, point))
                    {
                        // Si es la más cercana
                        if (newT < minT)
                        {
                            minT = newT;
                            minTObject = fig;
                            interseccion = point;
                        }
                        intersect = true;
                    }
                }
            }
            else
            {
                // Añade ambos hijos, ya que el padre no contenía figuras directamente
                nodosAVisitar.push_back(nodos[nodeIndex].secondChildOffset);
                nodosAVisitar.push_back(nodos[nodeIndex].firstChildOffset);
            }
        }
    }

    if (minTObject >= 0)
    {
        figura = figuras[minTObject];
    }
    return intersect;
}

std::shared_ptr<BoundingVolume::Node> BoundingVolume::recursiveBuild(std::vector<PrimitiveInfo> &primitiveInfo, int start, int end, int &totalNodes)
{
    std::shared_ptr<Node> nodo(new Node());
    int nPrimitives = end - start;
    totalNodes++;

    BoundingBox bounds;
    // Calcula la bounding box que contiene las de todas las primitive info
    for (int i = start; i < end; i++){
        bounds = Union(bounds, primitiveInfo[i].bound);
    }

    // Es nodo hoja
    if (nPrimitives == 1)
    {
        nodo->InitLeaf(bounds);
        for (int i = start; i < end; i++){
            nodo->primitives.push_back(primitiveInfo[i].primitiveIndex);
        }
        return nodo;
    }
    else
    {
        BoundingBox centroidBounds;
        for (int i = start; i < end; i++){
            centroidBounds = Union(centroidBounds, primitiveInfo[i].centroid);
        }
        int dim = centroidBounds.MaxAxe();
        int mid = (start + end) / 2;

        if (centroidBounds.getTop()[dim] == centroidBounds.getBottom()[dim])
        {
            nodo->InitLeaf(bounds);
            for (int i = start; i < end; i++)
            {
                nodo->primitives.push_back(primitiveInfo[i].primitiveIndex);
            }
            return nodo;
        }
        else
        {

            // Heurística de los centroides
            float pMid = (centroidBounds.getTop()[dim] + centroidBounds.getBottom()[dim]) / 2;
            PrimitiveInfo *midPtr = std::partition(&primitiveInfo[start], &primitiveInfo[end - 1] + 1,
                                                   [dim, pMid](const PrimitiveInfo &p) {
                                                       return p.centroid[dim] < pMid;
                                                   });

            mid = midPtr - &primitiveInfo[0];
            if (mid == start || mid == end)
            {
                mid = (start + end) / 2;
                std::nth_element(&primitiveInfo[start], &primitiveInfo[mid],
                                 &primitiveInfo[end - 1] + 1,
                                 [dim](const PrimitiveInfo &a, const PrimitiveInfo &b) {
                                     return a.centroid[dim] < b.centroid[dim];
                                 });
            }
        }

        // Invoca recursivamente la función para construir ambos hijos
        nodo->InitInterior(dim,
                           recursiveBuild(primitiveInfo, start, mid, totalNodes),
                           recursiveBuild(primitiveInfo, mid, end, totalNodes));
    }
    return nodo;
}

int BoundingVolume::getSize()
{
    return nodos.size();
}

int BoundingVolume::flattenTree(std::shared_ptr<Node> nodo, int &offset)
{
    nodos.push_back(LinearNode());
    int myOffset = offset++;
    nodos[myOffset].bound = nodo->bounds;
    nodos[myOffset].primitives = nodo->primitives;

    if (nodo->primitives.size() <= 0){
        nodos[myOffset].axis = nodo->splitAxis;
        nodos[myOffset].firstChildOffset = flattenTree(nodo->left, offset);
        nodos[myOffset].secondChildOffset = flattenTree(nodo->right, offset);
    }
    return myOffset;
}