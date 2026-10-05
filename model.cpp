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
        }else if (type == "vn") {
            vec3 n;
            for (int i : {0,1,2}) ss >> n[i];
            norms.push_back(normalized(n));}
        else if (type == "f") { // faces
            std::string token1, token2, token3;
            ss >> token1 >> token2 >> token3;

            auto parse_face_token = [](const std::string& token, int& v_out, int& n_out) {
                size_t first_slash = token.find('/');
                if (first_slash == std::string::npos) {
                    // Format: v
                    v_out = std::stoi(token) - 1;
                    n_out = -1;
                    return;
                }

                // Wierzchołek jest przed pierwszym slashem
                v_out = std::stoi(token.substr(0, first_slash)) - 1;

                size_t second_slash = token.find('/', first_slash + 1);
                if (second_slash == std::string::npos) {
                    // Format: v/vt (brak normalnej)
                    n_out = -1;
                } else {
                    // Format: v//vn lub v/vt/vn (normalna jest po drugim slashu)
                    std::string n_str = token.substr(second_slash + 1);
                    if (!n_str.empty()) {
                        n_out = std::stoi(n_str) - 1;
                    } else {
                        n_out = -1;
                    }
                }
            };

            int v1, v2, v3;
            int n1, n2, n3;

            parse_face_token(token1, v1, n1);
            parse_face_token(token2, v2, n2);
            parse_face_token(token3, v3, n3);

            faces.push_back({v1, v2, v3});
            faces_nrm.push_back({n1, n2, n3});
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
vec3 Model::normal(const int iface, const int nthvert) const {
    return norms[faces[iface][nthvert]];
}