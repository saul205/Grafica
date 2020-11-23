#include "TriangleMesh.h"

void TriangleMesh::transform(Transformation t)
{
    for (Triangle &tri : triangulos)
    {
        tri.transform(t);
    }
}

void TriangleMesh::read(string file)
{
    std::ifstream reader(file);
    string buffer;

    reader >> buffer;
    if (buffer != "ply")
    {
        std::cout << "Archivo no válido" << endl;
        return;
    }

    bool end = false;
    int properties[2] = {0, 0};
    int prop = 0;
    bool countCoor = false;
    float vertex, faces;
    while (!end)
    {
        reader >> buffer;
        if (buffer == "element")
        {
            reader >> buffer;

            if (buffer == "vertex")
            {
                reader >> vertex;
            }
            else if (buffer == "face")
            {
                reader >> faces;
                prop = 1;
            }

            countCoor = !countCoor;
        }
        else if (buffer == "end_header")
        {
            end = true;
        }
        else if (buffer == "format")
        {
            reader >> buffer;
            if (buffer != "ascii")
            {
                return;
            }
        }
        else
        {
            if (buffer == "property" && countCoor)
            {
                properties[prop]++;
            }
            getline(reader, buffer);
        }
    }

    if (properties[0] != 3)
    {
        std::cout << "No es válido" << endl;
        return;
    }

    std::vector<DotDir> puntos(vertex);
    float x, y, z;
    float minX = INFINITY, minY = INFINITY, minZ = INFINITY;
    float maxX = -INFINITY, maxY = -INFINITY, maxZ = -INFINITY;
    for (int i = 0; i < vertex; i++)
    {
        reader >> x >> y >> z;

        minX = std::min(minX, x);
        minY = std::min(minY, y);
        minZ = std::min(minZ, z);
        maxX = std::max(maxX, x);
        maxY = std::max(maxY, y);
        maxZ = std::max(maxZ, z);

        puntos[i] = DotDir(x, y, z, 1);
    }

    top = DotDir(maxX, maxY, maxZ, 1);
    bottom = DotDir(minX, minY, minZ, 1);
    center = bottom + 0.5f * (top - bottom);

    int nVertices;
    for (int i = 0; i < faces; i++)
    {
        reader >> nVertices;
        float ref, uno, dos;
        reader >> ref >> uno;
        for (int j = 1; j < nVertices - 1; j++)
        {
            reader >> dos;
            triangulos.push_back(Triangle(puntos[ref], puntos[uno], puntos[dos]));
            uno = dos;
        }
    }
}

Triangle TriangleMesh::operator[](int n)
{
    return triangulos[n];
}

float TriangleMesh::getSize() const
{
    return triangulos.size();
}

void TriangleMesh::move(DotDir newCenter)
{
    DotDir d = newCenter - center;

    Transformation t;
    t.translation(d[0], d[1], d[2]);

    transform(t);
}

void TriangleMesh::scale(float x, float y, float z)
{
    Transformation t;
    t.scale(x, y, z);

    transform(t);
}