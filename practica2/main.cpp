#include <iostream>
#include <string>
#include <fstream>

void leer(std::string nombre){

    std::fstream f(nombre);
    std::string format, max, trash, name;
    int width, height, maximo;
    float color_res;
    char c;

    f >> format;

    if(f.is_open()){
        f >> c;

        f >> max >> c >> name >> width >> height >> color_res;
    }

    std::cout << name << "\n" << format << "\n" << max.erase(0, 4) << "\n" << width << " | " << height << "\n" << color_res;


}

int main(){

    std::string file;
    std::cout << "Introduce una imagen: " << std::endl;
    std::cin >> file;
    leer(file);

    return 0;
}