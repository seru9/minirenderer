#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "model.h"

Model::Model(std::string filename){
    std::ifstream FILE(filename);
    if (!FILE.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    std::string line;
    
    while (std::getline(FILE, line)) {
        if (line.empty()) continue; 

        std::stringstream ss(line);
        std::string type;
        ss >> type;
        if (type == "v") { // points
            double x, y, z;
            ss >> x >> y >> z;
            points.push_back({x, y, z});
        }
        else if (type == "f") { // faces
            std::string token1, token2, token3;
            ss >> token1 >> token2 >> token3;

            auto parse_vertex = [](const std::string& token) {
                size_t slash_pos = token.find('/');
                std::string v_str = (slash_pos == std::string::npos) ? token : token.substr(0, slash_pos);
                return std::stoi(v_str) - 1; // Konwersja na int i przesunięcie na indeksowanie od 0
            };

            int v1 = parse_vertex(token1);
            int v2 = parse_vertex(token2);
            int v3 = parse_vertex(token3);
            faces.push_back({v1, v2, v3});
        }
    }
}
Model::Model(){
    
}
vec3 Model::vert(const int i) const {
    return points[i];
}

vec3 Model::vert(const int iface, const int nthvert) const {
    return points[faces[iface][nthvert]];
}