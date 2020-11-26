#include "DotDir.hpp"

DotDir::DotDir(float x, float y, float z, float w)
{
    c[0] = x;
    c[1] = y;
    c[2] = z;
    c[3] = w;
}

float DotDir::getX() const
{
    return c[0];
}

float DotDir::getY() const
{
    return c[1];
}

float DotDir::getZ() const
{
    return c[2];
}

float DotDir::getW() const
{
    return c[3];
}

void DotDir::setDotDir(float x, float y, float z, float w)
{
    c[0] = x;
    c[1] = y;
    c[2] = z;
    c[3] = w;
}

float DotDir::mod() const
{
    return sqrt(c[0] * c[0] + c[1] * c[1] + c[2] * c[2]);
}

string DotDir::toString() const
{
    return to_string(c[0]) + ", " + to_string(c[1]) + ", " + to_string(c[2]) + ", " + to_string(c[3]);
}

float DotDir::operator[](const int i) const
{
    return c[i];
}

float DotDir::operator[](int i)
{
    return c[i];
}

// Dotdir entre float
DotDir operator/(const DotDir &dd1, float div)
{
    return DotDir(dd1.c[0] / div, dd1.c[1] / div, dd1.c[2] / div, dd1.c[3]);
}

// Suma dotdirs
DotDir operator+(const DotDir &dd1, const DotDir &dd2)
{
    return DotDir(dd1.c[0] + dd2.c[0], dd1.c[1] + dd2.c[1], dd1.c[2] + dd2.c[2], dd1.c[3] + dd2.c[3]);
}

// Resta dotdirs
DotDir operator-(const DotDir &dd1, const DotDir &dd2)
{
    return DotDir(dd1.c[0] - dd2.c[0], dd1.c[1] - dd2.c[1], dd1.c[2] - dd2.c[2], dd1.c[3] - dd2.c[3]);
}

// Multiplicación float punto
DotDir operator*(const DotDir &dd1, float t)
{
    return DotDir(dd1.c[0] * t, dd1.c[1] * t, dd1.c[2] * t, dd1.c[3]);
}

// Multiplicación en el otro sentido float punto
DotDir operator*(float t, const DotDir &dd1)
{
    return DotDir(dd1.c[0] * t, dd1.c[1] * t, dd1.c[2] * t, dd1.c[3]);
}

// Devuelve el producto escalar de los vectores d1 y d2.
float dotProduct(const DotDir &dd1, const DotDir &dd2)
{
    return dd1.c[0] * dd2.c[0] + dd1.c[1] * dd2.c[1] + dd1.c[2] * dd2.c[2];
}

// Devuelve el producto vectorial de los vectores d1 y d2.
DotDir crossProduct(const DotDir &dd1, const DotDir &dd2)
{
    return DotDir(dd1.c[1] * dd2.c[2] - dd1.c[2] * dd2.c[1],
                  dd1.c[2] * dd2.c[0] - dd1.c[0] * dd2.c[2],
                  dd1.c[0] * dd2.c[1] - dd1.c[1] * dd2.c[0], 0);
}

// Normaliza dd
DotDir normalization(const DotDir &dd)
{
    float modulo = dd.mod();
    return DotDir(dd.c[0] / modulo, dd.c[1] / modulo, dd.c[2] / modulo, dd.c[3]);
}

// Calcula el ángulo entre dos vectores
float angle(const DotDir &dd1, const DotDir &dd2)
{
    float angulo = dotProduct(dd1, dd2);
    angulo = angulo / dd1.mod();
    angulo = angulo / dd2.mod();
    angulo = acos(angulo);
    return angulo;
}
