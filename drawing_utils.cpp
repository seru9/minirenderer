#include "drawing_utils.h"
#include "shapes_drawing_alghorithms.h"
#include <atomic>
#include <cmath>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <string>

static std::tuple<int,int, int> project(double_vec3 v, int width, int height){ // First of all, (x,y) is an orthogonal projection of the vector (x,y,z).
    return { (v.x + 1.) *  width/2,   // Second, since the input models are scaled to have fit in the [-1,1]^3 world coordinates,
             (v.y + 1.) * height/2,
             (v.z + 1.) *   255./2 }; 
}
/*
    Depth interpolation - Z 'buffer
*/
void draw(std::string filename, int width, int height, TGAImage& framebuffer, TGAImage& zbuffer, TGAColor color){
    Model model(filename);
    const auto& temp_points = model.get_points();
    const auto& temp_faces = model.get_faces();
    for(auto point : temp_points){
        auto [ax, ay, az] = project(point, width, height);
        framebuffer.set(ax, ay, color);
    }
    int drawn_faces = 0;
    for(auto face : temp_faces) {
        if (face[0] < 0 || face[0] >= static_cast<int>(temp_points.size()) ||
            face[1] < 0 || face[1] >= static_cast<int>(temp_points.size()) ||
            face[2] < 0 || face[2] >= static_cast<int>(temp_points.size())) {
            std::cerr << "Indeks poza zakresem! Face: " << face[0] << ", " << face[1] << ", " << face[2] << std::endl;
            continue;
        }
        double_vec3 point1 = temp_points[face[0]];
        double_vec3 point2 = temp_points[face[1]];
        double_vec3 point3 = temp_points[face[2]];
        auto [ax, ay, az] = project(point1, width, height); 
        auto [bx, by, bz] = project(point2, width, height);
        auto [cx, cy, cz] = project(point3, width, height);
        TGAColor rnd;
        for (int c=0; c<3; c++) rnd[c] = std::rand()%255;
        filled_triangle2(ax, ay, az, bx, by, bz, cx, cy, cz, framebuffer, zbuffer, rnd);
        drawn_faces++;
    }
    framebuffer.write_tga_file("framebuffer.tga");
    zbuffer.write_tga_file("zbuffer.tga");
}