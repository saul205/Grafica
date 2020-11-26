#ifndef TONE_MAPPER_H
#define TONE_MAPPER_H

#include "Transformation.hpp"
#include "math.h"
#include "Image.hpp"
#include "Lab.hpp"

class ToneMapper{
  
  private:

  public:

    ToneMapper(){}

    // Clamping a 1
    void clamping(Image& img);

    // Equalización
    void equalization(Image& img);

    // Clamp y equalización 
    void equalizeAndClamp(Image& img, const float clamping);

    // curva Gamma
    void gammaCurve(Image& img, const float gamma = 2.2);

    // curva Gamma + clamping
    void gammaCurveAndClamping(Image& img, const float clamping, const float gamma = 2.2);

    // Reinhard - versión antigua
    void Reinhard(Image& img, float exp, float white = 2, const float gamma = 2.2);

    // REinhard - versión nueva
    void ReinhardToneMapper(Image& img, const float a = 1.5f, const float delta = 0.18f);

    // Tone Mapper Uncharted 2 Filmic
    void Filmic(Image& img, float W = 10, float exp = 2.0);

  private:

    // Auxiliar para Reinhard, calcula la media
    float averageReinhardLab(std::vector<lab>& data, const float delta);

    // Auxiliar para Reinhard, aplica el tone mapping 
    void applyReinhard(std::vector<lab>& data, const float a, const float delta);

    // Ecuación auxiliar a FIlmic
    float eq(float x, float A = 1.5, float B = 0.5, float C = 0.1, float D = 0.2, float E = 0.02, float F = 0.3);

};

#endif