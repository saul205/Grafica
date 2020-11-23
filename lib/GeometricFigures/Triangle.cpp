#include "Triangle.h"

triangleVertexUV::triangleVertexUV(float _u1, float _v1, float _u2, float _v2, float _u3, float _v3)
{
    u1 = _u1;
    u2 = _u2;
    u3 = _u3;
    v1 = _v1;
    v2 = _v2;
    v3 = _v3;
}

Triangle::Triangle(const DotDir &_v0, const DotDir &_v1, const DotDir &_v2)
{
    v0 = _v0;
    v1 = _v1;
    v2 = _v2;
    v0v1 = v1 - v0;
    v0v2 = v2 - v0;
    normal = normalization(crossProduct(v0v1, v0v2));
    tvUV = triangleVertexUV();
    center = DotDir(
        (v1.getX() + v2.getX() + v0.getX()) / 3,
        (v1.getY() + v2.getY() + v0.getY()) / 3,
        (v1.getZ() + v2.getZ() + v0.getZ()) / 3,
        1);
}

Triangle::Triangle(const DotDir &_v0, const DotDir &_v1, const DotDir &_v2, const triangleVertexUV &_tvUV)
{
    v0 = _v0;
    v1 = _v1;
    v2 = _v2;
    v0v1 = v1 - v0;
    v0v2 = v2 - v0;
    tvUV = _tvUV;
}

// Möller–Trumbore intersection algorithm, decide si el rayo intersecta o no con el triángulo
bool Triangle::intersects(Ray ray, float &t, DotDir &p)
{

    const float epsilon = 10e-7;
    DotDir h = crossProduct(ray.getDir(), v0v2);
    float det = dotProduct(v0v1, h);

    if (det > -epsilon && det < epsilon)
        return false; // Rayo paralelo al triángulo

    float invDet = 1.0 / det;
    DotDir s = ray.getOrigen() - v0;
    float u = invDet * dotProduct(s, h);

    if (u < 0.0 || u > 1.0)
        return false;

    DotDir q = crossProduct(s, v0v1);
    float v = invDet * dotProduct(ray.getDir(), q);

    if (v < 0.0 || u + v > 1.0)
        return false;

    // Sabemos que intersecta el rayo al triángulo en este punto
    // porque las coordenadas baricentricas u y v cumplen las restricciones
    t = invDet * dotProduct(v0v2, q);

    if (t > epsilon)
    {
        p = ray.getOrigen() + t * ray.getDir();
        return true;
    }
    else
    {
        return false;
    }
}

// INtersection DEBE intersectar al triángulo
rgb Triangle::getTexture(const DotDir &p)
{

    // Primero de los triángulos v0v1v2
    DotDir abc = crossProduct(v0v1, v0v2);
    float abcArea = abc.mod() / 2;

    // Triángulo v0v1p
    DotDir abp = crossProduct(v0v1, p - v0);
    float abpArea = abp.mod() / 2;

    // Triángulo v2v0p
    DotDir cap = crossProduct(v0 - v2, p - v2);
    float capArea = cap.mod() / 2;

    float u = capArea / abcArea;
    float v = abpArea / abcArea;

    // A es 0,0 - B es height,0 - C es 0,width
    float he = textura.getHeight();
    float we = textura.getWidth();

    float xTexture = (1.0f - u - v) * tvUV.u1 + u * tvUV.u2 + v * tvUV.u3;
    float yTexture = (1.0f - u - v) * tvUV.v1 + u * tvUV.v2 + v * tvUV.v3;
    xTexture = xTexture * we;
    yTexture = yTexture * he;

    rgb dev = textura.getRGB(yTexture, xTexture);

    return dev;
}

DotDir Triangle::getCenter()
{
    return center;
}

void Triangle::getBase(DotDir interseccion, DotDir &base0, DotDir &base1, DotDir &base2)
{
    base2 = normal;
    base1 = crossProduct(base2, v0v1);
    base0 = crossProduct(base2, base1);

    if (base2.mod() != 1)
        base2 = normalization(base2);
    if (base0.mod() != 1)
        base0 = normalization(base0);
    if (base1.mod() != 1)
        base1 = normalization(base1);
}

BoundingBox Triangle::getBound()
{

    float maxX = max(max(v0.getX(), v1.getX()), v2.getX());
    float maxY = max(max(v0.getY(), v1.getY()), v2.getY());
    float maxZ = max(max(v0.getZ(), v1.getZ()), v2.getZ());

    float minX = min(min(v0.getX(), v1.getX()), v2.getX());
    float minY = min(min(v0.getY(), v1.getY()), v2.getY());
    float minZ = min(min(v0.getZ(), v1.getZ()), v2.getZ());

    return BoundingBox(
        DotDir(maxX, maxY, maxZ, 1),
        DotDir(minX, minY, minZ, 1));
}

void Triangle::transform(Transformation t)
{
    v0 = t * v0;
    v1 = t * v1;
    v2 = t * v2;

    v0v1 = v1 - v0;
    v0v2 = v2 - v0;
}