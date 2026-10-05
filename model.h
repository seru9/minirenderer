#pragma once
#include <future>
#include <iostream>
#include <fstream> 
#include <vector>
#include "geometry.h"
class Model{
    private:
        std::vector<vec3> points = {};
        std::vector<vec3> norms = {};
        std::vector<int_vec3> faces = {};
        std::vector<int_vec3> faces_nrm = {};
    public:
        Model();
        Model(std::string file);
        std::vector<double_vec3> get_points() const {return points;}
        std::vector<int_vec3> get_faces() const { return faces; }
        int nfaces() const { return faces.size();}
        int npoints() const { return points.size(); }
        vec3 vert(const int i) const;
        vec3 vert(const int iface, const int nthvert) const;
        vec3 normal(const int iface, const int nthvert) const;
        
};