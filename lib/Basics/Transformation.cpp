#include "Transformation.h"

Transformation::Transformation(float mat[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            matriz[i][j] = mat[i][j];
        }
    }
}

// Rellena la diagonal de la matriz con ceros
void Transformation::setZero()
{
    for (int i = 0; i < 4; ++i)
    {
        matriz[i][i] = 0;
    }
}

// Set de la matriz a la matriz identidad
void Transformation::identidad()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            matriz[i][j] = i == j ? 1 : 0;
        }
    }
}

// Set de la  matriz de traslacion
void Transformation::translation(float x, float y, float z)
{
    matriz[0][3] = x;
    matriz[1][3] = y;
    matriz[2][3] = z;
}

// Set de la  matriz de escala
void Transformation::scale(float x, float y, float z)
{
    matriz[0][0] = x;
    matriz[1][1] = y;
    matriz[2][2] = z;
}

// Set de la  matriz de rotacion eje X
void Transformation::rotationX(float theta)
{
    matriz[1][1] = cos(theta);
    matriz[1][2] = -sin(theta);
    matriz[2][2] = cos(theta);
    matriz[2][1] = sin(theta);
}

// Set de la  matriz de rotacion eje Y
void Transformation::rotationY(float theta)
{
    matriz[0][0] = cos(theta);
    matriz[2][0] = -sin(theta);
    matriz[2][2] = cos(theta);
    matriz[0][2] = sin(theta);
}

// Set de la  matriz de rotacion eje Z
void Transformation::rotationZ(float theta)
{
    matriz[0][0] = cos(theta);
    matriz[0][1] = -sin(theta);
    matriz[1][1] = cos(theta);
    matriz[1][0] = sin(theta);
}

// Set de la  matriz de cambio de base formada por los vectores u,v,w y centrada
// en el punto o.
void Transformation::changeBase(const DotDir &u, const DotDir &v, const DotDir &w, const DotDir &o)
{
    matriz[0][0] = u.getX();
    matriz[1][0] = u.getY();
    matriz[2][0] = u.getZ();
    matriz[0][1] = v.getX();
    matriz[1][1] = v.getY();
    matriz[2][1] = v.getZ();
    matriz[0][2] = w.getX();
    matriz[1][2] = w.getY();
    matriz[2][2] = w.getZ();
    matriz[0][3] = o.getX();
    matriz[1][3] = o.getY();
    matriz[2][3] = o.getZ();
}

string Transformation::toString() const
{
    string dev = to_string(matriz[0][0]) + ", " + to_string(matriz[0][1]) + ", " + to_string(matriz[0][2]) + ", " + to_string(matriz[0][3]) + '\n' + to_string(matriz[1][0]) + ", " + to_string(matriz[1][1]) + ", " + to_string(matriz[1][2]) + ", " + to_string(matriz[1][3]) + '\n' + to_string(matriz[2][0]) + ", " + to_string(matriz[2][1]) + ", " + to_string(matriz[2][2]) + ", " + to_string(matriz[2][3]) + '\n' + to_string(matriz[3][0]) + ", " + to_string(matriz[3][1]) + ", " + to_string(matriz[3][2]) + ", " + to_string(matriz[3][3]);
    return dev;
}

// Devuelve la matriz transpuesta
Transformation transpuesta(const Transformation &t1)
{
    Transformation t;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            t.matriz[i][j] = t1.matriz[j][i];
        }
    }
    return t;
}

// Devuelve el producto de transformaciones (matrices)
Transformation operator*(const Transformation &t1, const Transformation &t2)
{
    Transformation producto;
    producto.setZero();
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            for (int k = 0; k < 4; ++k)
            {
                producto.matriz[i][j] += t1.matriz[i][k] * t2.matriz[k][j];
            }
        }
    }
    return producto;
}

// Devuelve producto de transformacion (matriz) por vector
DotDir operator*(const Transformation &t, const DotDir &dd)
{
    DotDir producto(t.matriz[0][0] * dd.getX() + t.matriz[0][1] * dd.getY() + t.matriz[0][2] * dd.getZ() + t.matriz[0][3] * dd.getW(),
                    t.matriz[1][0] * dd.getX() + t.matriz[1][1] * dd.getY() + t.matriz[1][2] * dd.getZ() + t.matriz[1][3] * dd.getW(),
                    t.matriz[2][0] * dd.getX() + t.matriz[2][1] * dd.getY() + t.matriz[2][2] * dd.getZ() + t.matriz[2][3] * dd.getW(),
                    t.matriz[3][0] * dd.getX() + t.matriz[3][1] * dd.getY() + t.matriz[3][2] * dd.getZ() + t.matriz[3][3] * dd.getW());
    return producto;
}

// Calculo del determinante de 3x3
float Sarrus(float mat[3][3])
{
    return mat[0][0] * mat[1][1] * mat[2][2] + mat[1][0] * mat[2][1] * mat[0][2] + mat[2][0] * mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1] * mat[2][0] - mat[1][2] * mat[2][1] * mat[0][0] - mat[2][2] * mat[0][1] * mat[1][0];
}

// Obtenemos los cofactores para el calculo del determinante 4x4
void cofactor(float mat[4][4], float cof[3][3], int q)
{
    int i = 0, j = 0;

    for (int x = 1; x < 4; x++)
    {
        j = 0;
        for (int y = 0; y < 4; y++)
        {
            if (y != q)
            {
                //Guardamos en cof el cofactor actual
                cof[i][j++] = mat[x][y];
            }
        }
        i++;
    }
}

// Calculo del determinante 4x4
float determinante(float mat[4][4])
{
    float det = 0;
    float cof[3][3];
    int s = 1;

    for (int q = 0; q < 4; q++)
    {
        cofactor(mat, cof, q);
        //Simplificamos el calculo del determinante usando sarrus para los 3x3
        det += s * mat[0][q] * Sarrus(cof);
        s = -s;
    }
    return det;
}

//Devuelve la inversa de una transformación
Transformation inverse(const Transformation &t1)
{

    const int n = 4;
    float matrix[4][4], I[4][4];
    double factor;

    //Copiamos la matriz en otra matriz auxiliar para no modificar la transformación y generamos la identidad.
    for (int j = 0; j < n; j++)
    {
        for (int z = 0; z < n; z++)
        {
            I[j][z] = 0;
            matrix[j][z] = t1.matriz[j][z];
        }

        I[j][j] = 1;
    }

    if (determinante(matrix) == 0)
    {
        cout << "No es inversible" << endl;
        return Transformation();
    }

    //Recorremos las filas
    for (int i = 0; i < n; i++)
    {

        int j = i + 1;
        //Se busca una fila que no tenga 0 en la componente de la diagonal necesaria y se permuta
        while (matrix[i][i] == 0)
        {
            if (matrix[j][i] != 0)
            {
                for (int z = 0; z < 4; z++)
                {
                    float aux = matrix[i][z];
                    matrix[i][z] = matrix[j][z];
                    matrix[j][z] = aux;

                    aux = I[i][z];
                    I[i][z] = I[j][z];
                    I[j][z] = aux;
                }
            }

            j++;
        }

        //Hardcodear si se necesita velocidad
        //Obtenemos 1 en la diagonal
        factor = matrix[i][i];
        for (int z = 0; z < 4; z++)
        {
            matrix[i][z] /= factor;
            I[i][z] /= factor;
        }

        //Obtenemos 0 en la componente correspondiente en las filas inferiores
        for (int x = i + 1; x < 4; x++)
        {
            factor = matrix[x][i] / matrix[i][i];
            for (int z = 0; z < 4; z++)
            {
                matrix[x][z] -= factor * matrix[i][z];
                I[x][z] -= factor * I[i][z];
            }
        }
    }

    // Tenemos matriz diagonal superior, obtenemos 0s en la parte superior
    for (int i = n - 1; i > 0; i--)
    {
        for (int j = i - 1; j >= 0; j--)
        {
            factor = matrix[j][i] / matrix[i][i];
            for (int z = 0; z < 4; z++)
            {
                matrix[j][z] -= factor * matrix[i][z];
                I[j][z] -= factor * I[i][z];
            }
        }
    }

    return Transformation(I);
}