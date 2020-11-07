#ifndef TONE_MAPPER_H
#define TONE_MAPPER_H

#include "Transformation.h"
#include "math.h"
#include "Image.h"
#include "Lab.h"

class ToneMapper{
  
  private:

  public:

    ToneMapper(){}

    void clamping(Image& img){
        const float maximo = img.getMaximo();
        for(int i = 0; i < img.getHeight(); ++i){
            for(int j = 0; j < img.getWidth(); ++j){
                rgb colores = img.get(i * img.getWidth() + j);
                
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
                rgb colores = img.get(i * img.getWidth() + j);
                
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
                rgb colores = img.get(i * img.getWidth() + j);
                
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
                rgb colores = img.get(i * img.getWidth() + j);
                
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
                rgb colores = img.get(i * img.getWidth() + j);
                
                colores.r = (colores.r <= clamping) ? pow(colores.r / clamping, gamma)  : 1;
                colores.g = (colores.g <= clamping) ? pow(colores.g / clamping, gamma)  : 1;
                colores.b = (colores.b <= clamping) ? pow(colores.b / clamping, gamma)  : 1;

                img.setRGB(i * img.getWidth() + j, colores);
            }
        }
    }

    void Reinhard(Image& img, float exp, float white = 2, const float gamma = 2.2){
        for(int i = 0; i < img.getHeight(); ++i){
            for(int j = 0; j < img.getWidth(); ++j){
                rgb colores = img.get(i * img.getWidth() + j);
                
                colores.r = pow(colores.r * exp * (1 + colores.r * exp / white) / (1 + colores.r / exp), 1 / gamma);
                colores.g = pow(colores.g * exp * (1 + colores.g * exp / white) / (1 + colores.g / exp), 1 / gamma);
                colores.b = pow(colores.b * exp * (1 + colores.b * exp / white) / (1 + colores.b / exp), 1 / gamma);

                img.setRGB(i * img.getWidth() + j, colores);
            }
        }
    }

    void ReinhardToneMapper(Image& img, const float a = 0.12f, const float delta = 0.18){
        std::vector<lab> data;
        RGBToLab(img, data);
        applyReinhard(data, a, delta);
        LabToRGB(img, data);
    }

    float averageReinhardLab(std::vector<lab> data, const float delta){

        float suma = 0.0f;
        for(unsigned int i = 0; i < data.size(); ++i){
            suma = suma + log(delta + data[i].l);
        }
        return exp(suma) / ((float) data.size());
    }

    void applyReinhard(std::vector<lab> data, const float a, const float delta){

        float lw = averageReinhardLab(data, delta);
        for(unsigned int i = 0; i < data.size(); ++i){
            data[i].l = data[i].l * a / lw;
        }

        float lWhite = -1.0f;
        for(unsigned int i = 0; i < data.size(); ++i){
            if(data[i].l > lWhite){
                lWhite = data[i].l;
            }
        }
        for(unsigned int i = 0; i < data.size(); ++i){
            data[i].l = (data[i].l * (1.0f + (data[i].l / pow(lWhite,2.0f))))/ (data[i].l + 1.0f);
        }
    }

    float eq(float x, 
             float A = 1.5, 
             float B = 0.5, 
             float C = 0.1, 
             float D = 0.2, 
             float E = 0.02, 
             float F = 0.3){

        return ((x *(A*x + C*B) + D*E) / (x*(A*x + B) + D*F)) - E/F;
    } 

    void Filmic(Image& img, float W = 10, float exp = 2.0) {
        for(int i = 0; i < img.getHeight(); ++i){
            for(int j = 0; j < img.getWidth(); ++j){
                rgb colores = img.get(i * img.getWidth() + j);
                
                colores.r = (exp * eq(colores.r)) * (1 / eq(W));
                colores.g = (exp * eq(colores.g)) * (1 / eq(W));
                colores.b = (exp * eq(colores.b)) * (1 / eq(W));

                img.setRGB(i * img.getWidth() + j, colores);
            }
        }
    }

};

#endif