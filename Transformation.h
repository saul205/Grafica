#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "DotDir.h"

class Transformation{
  private:
    float matriz[4][4] = {1,0,0,0,
                          0,1,0,0,
                          0,0,1,0,
                          0,0,0,1};
  public:

    Transformation() {}

    friend Transformation operator*(const Transformation& t1, const Transformation& t2);
    friend DotDir operator*(const Transformation& t, const DotDir& dd);
    friend Transformation transpuesta(const Transformation& t1);
    
    // Rellena la diagonal de la matriz con ceros
    void setZero(){
      for(int i = 0; i < 4; ++i){
        matriz[i][i] = 0;
      }
    }

    // Set de la matriz a la matriz identidad
    void identidad(){
      for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
          matriz[i][j] = i == j ? 1 : 0;
        }
      }
    }

    // Set de la  matriz de traslacion
    void translation(float x, float y, float z){
      matriz[0][3] = x;
      matriz[1][3] = y;
      matriz[2][3] = z;
    }

    // Set de la  matriz de escala
    void scale(float x, float y, float z){
      matriz[0][0] = x;
      matriz[1][1] = y;
      matriz[2][2] = z;
    }

    // Set de la  matriz de rotacion eje X
    void rotationX(float theta){
      matriz[1][1] = cos(theta);
      matriz[1][2] = -sin(theta);
      matriz[2][2] = cos(theta);
      matriz[2][1] = sin(theta);
    }

    // Set de la  matriz de rotacion eje Y
    void rotationY(float theta){
      matriz[0][0] = cos(theta);
      matriz[2][0] = -sin(theta);
      matriz[2][2] = cos(theta);
      matriz[0][2] = sin(theta);
    }

    // Set de la  matriz de rotacion eje Z
    void rotationZ(float theta){
      matriz[0][0] = cos(theta);
      matriz[0][1] = -sin(theta);
      matriz[1][1] = cos(theta);
      matriz[1][0] = sin(theta);
    }

    // Set de la  matriz de cambio de base formada por los vectores u,v,w y centrada
    // en el punto o.
    void changeBase(const DotDir& u, const DotDir& v, const DotDir& w, const DotDir& o){
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

    string toString() const{
      string dev =   to_string(matriz[0][0]) + ", " + to_string(matriz[0][1]) + ", " + to_string(matriz[0][2]) + ", " + to_string(matriz[0][3]) + '\n' 
                  +  to_string(matriz[1][0]) + ", " + to_string(matriz[1][1]) + ", " + to_string(matriz[1][2]) + ", " + to_string(matriz[1][3]) + '\n' 
                  +  to_string(matriz[2][0]) + ", " + to_string(matriz[2][1]) + ", " + to_string(matriz[2][2]) + ", " + to_string(matriz[2][3]) + '\n' 
                  +  to_string(matriz[3][0]) + ", " + to_string(matriz[3][1]) + ", " + to_string(matriz[3][2]) + ", " + to_string(matriz[3][3]);
      return dev;
    }
};


// Devuelve la matriz transpuesta
Transformation transpuesta(const Transformation& t1){
      Transformation t;
      int i,j;
      for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++){
          t.matriz[i][j] = t1.matriz[j][i];
        }
      }
      return t;
}

// Devuelve el producto de transformaciones (matrices)
Transformation operator*(const Transformation& t1, const Transformation& t2){
  Transformation producto;
  producto.setZero();
  for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			for(int k=0; k < 4; ++k) {
				producto.matriz[i][j] += t1.matriz[i][k] * t2.matriz[k][j];
			}
		}
	}
  return producto;
}

// Devuelve producto de transformacion (matriz) por vector
DotDir operator*(const Transformation& t, const DotDir& dd){
  DotDir producto(t.matriz[0][0]*dd.getX() + t.matriz[0][1]*dd.getY() + t.matriz[0][2]*dd.getZ() + t.matriz[0][3]*dd.getW(), 
                  t.matriz[1][0]*dd.getX() + t.matriz[1][1]*dd.getY() + t.matriz[1][2]*dd.getZ() + t.matriz[1][3]*dd.getW(), 
                  t.matriz[2][0]*dd.getX() + t.matriz[2][1]*dd.getY() + t.matriz[2][2]*dd.getZ() + t.matriz[2][3]*dd.getW(), 
                  t.matriz[3][0]*dd.getX() + t.matriz[3][1]*dd.getY() + t.matriz[3][2]*dd.getZ() + t.matriz[3][3]*dd.getW());
  return producto;
}

#endif