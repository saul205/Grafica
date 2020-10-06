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
                
                colores.r = colores.r <= max ? colores.r : max;
                colores.g = colores.g <= max ? colores.g : max;
                colores.b = colores.b <= max ? colores.b : max;

                img.setRGB(i * img.getWidth() + j, colores);
            }
        }
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
    }

    void gammaCurveAndClamping(Image& img, const float clamping, const float gamma = 2.2){
        const float max = img.getMax();
        for(int i = 0; i < img.getHeight(); ++i){
            for(int j = 0; j < img.getWidth(); ++j){
                Image::rgb colores = img.get(i * img.getWidth() + j);
                
                colores.r = (colores.r / max <= clamping) ? pow(colores.r / max, gamma) / clamping : 1;
                colores.g = (colores.g / max <= clamping) ? pow(colores.g / max, gamma) / clamping : 1;
                colores.b = (colores.b / max <= clamping) ? pow(colores.b / max, gamma) / clamping : 1;

                img.setRGB(i * img.getWidth() + j, colores);
            }
        }
    }

};

#endif