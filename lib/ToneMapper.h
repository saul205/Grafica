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
        const float max = img.getMax();
        for(int i = 0; i < img.getHeight(); ++i){
            for(int j = 0; j < img.getWidth(); ++j){
                Image::rgb colores = img.get(i * img.getWidth() + j);
                
                colores.r = colores.r <= 1 ? colores.r : 1 ;
                colores.g = colores.g <= 1 ? colores.g : 1 ;
                colores.b = colores.b <= 1 ? colores.b : 1 ;

                img.setRGB(i * img.getWidth() + j, colores);
            }
        }

        img.setMax(1);
    }

    void equalization(Image& img){
        const float max = img.getMax();
        for(int i = 0; i < img.getHeight(); ++i){
            for(int j = 0; j < img.getWidth(); ++j){
                Image::rgb colores = img.get(i * img.getWidth() + j);
                
                colores.r = colores.r / max;
                colores.g = colores.g / max;
                colores.b = colores.b / max;

                if(colores.g == max){
                    std::cout << i * img.getWidth() + j << std::endl;
                }

                img.setRGB(i * img.getWidth() + j, colores);
            }
        }

        img.setMax(1);
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

        img.setMax(1);
    }

    void gammaCurve(Image& img, const float gamma = 2.2){
        const float max = img.getMax();
        for(int i = 0; i < img.getHeight(); ++i){
            for(int j = 0; j < img.getWidth(); ++j){
                Image::rgb colores = img.get(i * img.getWidth() + j);
                
                colores.r = pow((colores.r / max), gamma); 
                colores.g = pow((colores.g / max), gamma); 
                colores.b = pow((colores.b / max), gamma); 

                img.setRGB(i * img.getWidth() + j, colores);
            }
        }

        img.setMax(1);
    }

    void gammaCurveAndClamping(Image& img, const float clamping, const float gamma = 2.2){
        const float max = img.getMax();
        for(int i = 0; i < img.getHeight(); ++i){
            for(int j = 0; j < img.getWidth(); ++j){
                Image::rgb colores = img.get(i * img.getWidth() + j);
                
                colores.r = (colores.r <= clamping) ? pow(colores.r / clamping, gamma)  : 1;
                colores.g = (colores.g <= clamping) ? pow(colores.g / clamping, gamma)  : 1;
                colores.b = (colores.b <= clamping) ? pow(colores.b / clamping, gamma)  : 1;

                img.setRGB(i * img.getWidth() + j, colores);
            }
        }

        img.setMax(1);
    }

};

#endif