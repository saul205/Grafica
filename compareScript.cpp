#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct color{
    float r, g, b;

    float operator-(color b){
        return abs(this->r - b.r) + abs(this->g - b.g) + abs(this->b - b.b);
    }
};

vector<color> leer(std::string nombre){

    std::ifstream f(nombre);
    std::string format;
    std::string linea;
    int numDatos = 0;

    int width, height, color_res;
    float maximo = 1.0f;
    bool maxFound = false; // True si se ha encontrado el comentario MAX

    if(f.is_open()){
        getline(f, format);

        while(numDatos != 2){
            getline(f, linea);
            if(linea[0] != '#'){                // Altura y anchura
                if(numDatos == 0){
                    std::istringstream iss(linea);
                    iss >> width;
                    iss >> height;
                    ++numDatos;
                } else {                        // Color resolution
                    ++numDatos;
                }
            }
        }

        // Lee el resto de comentarios que puedan existir
        int firstDigit;
        firstDigit = f.peek();
        while(!std::isdigit(firstDigit)){
            getline(f, linea);
        }

        vector<color> vec(width * height);
        float rgbR = 0, rgbG = 0, rgbB = 0;
        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                f >> rgbR;
                f >> rgbG;
                f >> rgbB;
                vec.push_back({rgbR, rgbG, rgbB});
            }
        }
        return vec;
    }
    else{
        std::cout << "Error en la lectura" << std::endl;
        return vector<color>();
    }
    
}

void compare(vector<color>& i1, vector<color>& i2){

    if(size(i1) != size(i2)){
        cout << "incompatibles" << endl;
        return;
    }

    float diff = 0;
    for(int i = 0; i < size(i1); i++){
        diff += abs(i1[i] - i2[i]);
    }

    cout << "Error total: " << diff << endl;

}

void main(int argc, char** argv){
    if(argc != 3){
        cout << "Ejecución: compareScript <rutaImagen1> <rutaImagen2>" << endl;
    }

    vector<color> i1 = leer(argv[1]);
    vector<color> i2 = leer(argv[2]);

    compare(i1, i2);
}