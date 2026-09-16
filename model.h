#pragma once
#include <future>
#include <iostream>
#include <fstream> 
#include <vector>
#include "geometry.h"
class Model{
    private:
        std::vector<double_vec3> points = {};
        std::vector<int_vec3> faces = {};
    public:
        Model(std::string file);
        std::vector<double_vec3> get_points() const {return points;}
        std::vector<int_vec3> get_faces() const { return faces; }

        
};