#ifndef TONE_MAPPER_H
#define TONE_MAPPER_H

#include "../lib/Transformation.h"
#include "math.h"

class ToneMapper{
  
  private:

  public:

    ToneMapper(){}

    void clamping(Image& img){
        for(int i = 0; i < img.getRows(); ++i){
            for(int j = 0; j < img.getCols(); ++j){
                img.mat[i][j] = (img.mat[i][j] <= 1) ? img.mat[i][j] : 1;
            }
        }
    }

    void equalization(Image& img){
        const float max = img.getMaxValue();
        for(int i = 0; i < img.getRows(); ++i){
            for(int j = 0; j < img.getCols(); ++j){
                img.mat[i][j] /= max;
            }
        }
    }

    void equalizeAndClamp(Image& img, const float clamping){
        for(int i = 0; i < img.getRows(); ++i){
            for(int j = 0; j < img.getCols(); ++j){
                img.mat[i][j] = (img.mat[i][j] <= clamping) ? img.mat[i][j] / clamping : 1;
            }
        }
    }

    void gammaCurve(Image& img, const float gamma = 2.2){
        const float max = img.getMaxValue();
        for(int i = 0; i < img.getRows(); ++i){
            for(int j = 0; j < img.getCols(); ++j){
                img.mat[i][j] /= max;
                img.mat[i][j] = pow(img.mat[i][j], gamma);
            }
        }
    }

    void gammaCurveAndClamping(Image& img, const float clamping, const float gamma = 2.2){
        const float max = img.getMaxValue();
        for(int i = 0; i < img.getRows(); ++i){
            for(int j = 0; j < img.getCols(); ++j){
                img.mat[i][j] = (img.mat[i][j] <= clamping) ? img.mat[i][j] / clamping : 1;
                img.mat[i][j] = pow(img.mat[i][j], gamma);
            }
        }
    }

};

#endif