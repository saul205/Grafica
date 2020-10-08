#ifndef TONE_MAPPER_H
#define TONE_MAPPER_H

#include "Transformation.h"
#include "math.h"
#include "Image.h"

class ToneMapper{
  
  private:

  public:

    ToneMapper(){}

    void clamping(Image& img){
        const float maximo = img.getMaximo();
        for(int i = 0; i < img.getHeight(); ++i){
            for(int j = 0; j < img.getWidth(); ++j){
                Image::rgb colores = img.get(i * img.getWidth() + j);
                
                colores.r = colores.r <= 1 ? colores.r : 1 ;
                colores.g = colores.g <= 1 ? colores.g : 1 ;
                colores.b = colores.b <= 1 ? colores.b : 1 ;

                img.setRGB(i * img.getWidth() + j, colores);
            }
        }
    }

    void equalization(Image& img){

        // Buscar máximo o cogerlo directamente?
        const float maximo = img.getMaximo();

        for(int i = 0; i < img.getHeight(); ++i){
            for(int j = 0; j < img.getWidth(); ++j){
                Image::rgb colores = img.get(i * img.getWidth() + j);
                
                colores.r = colores.r / maximo;
                colores.g = colores.g / maximo;
                colores.b = colores.b / maximo;

                if(colores.g == maximo){
                    std::cout << i * img.getWidth() + j << std::endl;
                }

                img.setRGB(i * img.getWidth() + j, colores);
            }
        }
    }

    void equalizeAndClamp(Image& img, const float clamping){
        for(int i = 0; i < img.getHeight(); ++i){
            for(int j = 0; j < img.getWidth(); ++j){
                Image::rgb colores = img.get(i * img.getWidth() + j);
                
                colores.r = colores.r <= clamping ? colores.r / clamping : 1;
                colores.g = colores.g <= clamping ? colores.g / clamping : 1;
                colores.b = colores.b <= clamping ? colores.b / clamping : 1;

                img.setRGB(i * img.getWidth() + j, colores);
            }
        }
    }

    void gammaCurve(Image& img, const float gamma = 2.2){
        const float maximo = img.getMaximo();
        for(int i = 0; i < img.getHeight(); ++i){
            for(int j = 0; j < img.getWidth(); ++j){
                Image::rgb colores = img.get(i * img.getWidth() + j);
                
                colores.r = pow((colores.r / maximo), gamma); 
                colores.g = pow((colores.g / maximo), gamma); 
                colores.b = pow((colores.b / maximo), gamma); 

                img.setRGB(i * img.getWidth() + j, colores);
            }
        }
    }

    void gammaCurveAndClamping(Image& img, const float clamping, const float gamma = 2.2){
        const float maximo = img.getMaximo();
        for(int i = 0; i < img.getHeight(); ++i){
            for(int j = 0; j < img.getWidth(); ++j){
                Image::rgb colores = img.get(i * img.getWidth() + j);
                
                colores.r = (colores.r <= clamping) ? pow(colores.r / clamping, gamma)  : 1;
                colores.g = (colores.g <= clamping) ? pow(colores.g / clamping, gamma)  : 1;
                colores.b = (colores.b <= clamping) ? pow(colores.b / clamping, gamma)  : 1;

                img.setRGB(i * img.getWidth() + j, colores);
            }
        }
    }

};

#endif