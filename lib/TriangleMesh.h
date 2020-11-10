#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include <fstream>
#include "DotDir.h"
#include <vector>
#include "Triangle.h"
#include <iostream>

class TriangleMesh{

    private:

        std::vector<Triangle> triangulos;
        DotDir center, top, bottom;

    public:

        TriangleMesh(){}

        void read(string file){
            std::ifstream reader(file);
            string buffer;

            reader >> buffer;
            if(buffer != "ply"){
                std::cout << "Archivo no válido" << endl;
                return;
            }

            bool end = false;
            int coordinates = 0, elC = 0;
            bool countCoor = false;
            float vertex, faces;
            while(!end){
                reader >> buffer;
                if(buffer == "element"){
                    if(elC == 0){
                        reader >> buffer >> vertex;
                        elC ++;
                    } 
                    else reader >> buffer >> faces;
                    
                    countCoor = !countCoor;
                }else if(buffer == "end_header"){
                    end = true;
                }
                else if(buffer == "format"){
                    reader >> buffer;
                    if(buffer != "ascii"){
                        return;
                    }
                }
                else{
                    if(buffer == "property" && countCoor){
                        coordinates ++;
                    }
                    getline(reader, buffer);
                }
            }

            if(coordinates != 3){
                std::cout << "No es un triangulo" << endl;
                return;
            }

            std::vector<DotDir> puntos(vertex);
            float x, y, z;
            for(int i = 0; i < vertex; i++){
                reader >> x >> y >> z;
                puntos[i] = DotDir(x, y, z, 1);
            }

            int nVertices;
            for(int i = 0; i < faces; i++){
                reader >> nVertices;
                float ref, uno, dos;
                reader >> ref >> uno;
                for(int j = 1; j < nVertices - 1; j ++){
                    reader >> dos; 
                    triangulos.push_back(Triangle(puntos[ref], puntos[uno], puntos[dos]));
                    uno = dos;
                }
            }
        }

        Triangle operator[](int n) {
            return triangulos[n];
        } 

        float getSize() const {
            return triangulos.size();
        }
};

#endif