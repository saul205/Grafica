#include "ToneMapper.hpp"

void ToneMapper::clamping(Image &img)
{

    for (int i = 0; i < img.getHeight(); ++i)
    {
        for (int j = 0; j < img.getWidth(); ++j)
        {
            rgb colores = img.get(i * img.getWidth() + j);

            colores.r = colores.r <= 1 ? colores.r : 1;
            colores.g = colores.g <= 1 ? colores.g : 1;
            colores.b = colores.b <= 1 ? colores.b : 1;

            img.setRGB(i * img.getWidth() + j, colores);
        }
    }
}

void ToneMapper::equalization(Image &img)
{

    // Buscar máximo o cogerlo directamente?
    const float maximo = img.getMaximo();

    for (int i = 0; i < img.getHeight(); ++i)
    {
        for (int j = 0; j < img.getWidth(); ++j)
        {
            rgb colores = img.get(i * img.getWidth() + j);

            colores.r = colores.r / maximo;
            colores.g = colores.g / maximo;
            colores.b = colores.b / maximo;

            if (colores.g == maximo)
            {
                std::cout << i * img.getWidth() + j << std::endl;
            }

            img.setRGB(i * img.getWidth() + j, colores);
        }
    }
}

void ToneMapper::equalizeAndClamp(Image &img, const float clamping)
{
    for (int i = 0; i < img.getHeight(); ++i)
    {
        for (int j = 0; j < img.getWidth(); ++j)
        {
            rgb colores = img.get(i * img.getWidth() + j);

            colores.r = colores.r <= clamping ? colores.r / clamping : 1;
            colores.g = colores.g <= clamping ? colores.g / clamping : 1;
            colores.b = colores.b <= clamping ? colores.b / clamping : 1;

            img.setRGB(i * img.getWidth() + j, colores);
        }
    }
}

void ToneMapper::gammaCurve(Image &img, const float gamma)
{
    const float maximo = img.getMaximo();
    for (int i = 0; i < img.getHeight(); ++i)
    {
        for (int j = 0; j < img.getWidth(); ++j)
        {
            rgb colores = img.get(i * img.getWidth() + j);

            colores.r = pow((colores.r / maximo), gamma);
            colores.g = pow((colores.g / maximo), gamma);
            colores.b = pow((colores.b / maximo), gamma);

            img.setRGB(i * img.getWidth() + j, colores);
        }
    }
}

void ToneMapper::gammaCurveAndClamping(Image &img, const float clamping, const float gamma)
{

    for (int i = 0; i < img.getHeight(); ++i)
    {
        for (int j = 0; j < img.getWidth(); ++j)
        {
            rgb colores = img.get(i * img.getWidth() + j);

            colores.r = (colores.r <= clamping) ? pow(colores.r / clamping, gamma) : 1;
            colores.g = (colores.g <= clamping) ? pow(colores.g / clamping, gamma) : 1;
            colores.b = (colores.b <= clamping) ? pow(colores.b / clamping, gamma) : 1;

            img.setRGB(i * img.getWidth() + j, colores);
        }
    }
}

void ToneMapper::Reinhard(Image &img, float exp, float white, const float gamma)
{
    for (int i = 0; i < img.getHeight(); ++i)
    {
        for (int j = 0; j < img.getWidth(); ++j)
        {
            rgb colores = img.get(i * img.getWidth() + j);

            colores.r = pow(colores.r * exp * (1 + colores.r * exp / white) / (1 + colores.r / exp), 1 / gamma);
            colores.g = pow(colores.g * exp * (1 + colores.g * exp / white) / (1 + colores.g / exp), 1 / gamma);
            colores.b = pow(colores.b * exp * (1 + colores.b * exp / white) / (1 + colores.b / exp), 1 / gamma);

            img.setRGB(i * img.getWidth() + j, colores);
        }
    }
}

void ToneMapper::ReinhardToneMapper(Image &img, const float a, const float delta)
{
    std::vector<lab> data;
    RGBToLab(img, data);
    applyReinhard(data, a, delta);
    LabToRGB(img, data);
}

float ToneMapper::averageReinhardLab(std::vector<lab> &data, const float delta)
{

    float suma = 0.0f;
    for (unsigned int i = 0; i < data.size(); ++i)
    {
        suma = suma + logf(delta + data[i].l);
    }
    return expf(suma / ((float)data.size()));
}

void ToneMapper::applyReinhard(std::vector<lab> &data, const float a, const float delta)
{

    float lw = averageReinhardLab(data, delta);
    for (unsigned int i = 0; i < data.size(); ++i)
    {
        data[i].l = (data[i].l * a) / lw;
    }

    float lWhite = -1.0f;
    for (unsigned int i = 0; i < data.size(); ++i)
    {
        if (data[i].l > lWhite)
        {
            lWhite = data[i].l;
        }
    }

    for (unsigned int i = 0; i < data.size(); ++i)
    {
        data[i].l = (data[i].l * (1.0f + (data[i].l / pow(lWhite, 2.0f)))) / (data[i].l + 1.0f);
    }
}

float ToneMapper::eq(float x, float A, float B, float C, float D, float E, float F)
{

    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

void ToneMapper::Filmic(Image &img, float W, float exp)
{
    for (int i = 0; i < img.getHeight(); ++i)
    {
        for (int j = 0; j < img.getWidth(); ++j)
        {
            rgb colores = img.get(i * img.getWidth() + j);

            colores.r = (exp * eq(colores.r)) * (1 / eq(W));
            colores.g = (exp * eq(colores.g)) * (1 / eq(W));
            colores.b = (exp * eq(colores.b)) * (1 / eq(W));

            img.setRGB(i * img.getWidth() + j, colores);
        }
    }
}
