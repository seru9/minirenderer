#include "drawing_utils.h"
#include "geometry.h"
#include "shapes_drawing_alghorithms.h"
#include <atomic>
#include <cmath>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <string>
#include <limits>
#include <vector>

static std::tuple<int,int, int> project(double_vec3 v, int width, int height){ // First of all, (x,y) is an orthogonal projection of the vector (x,y,z).
    return { (v.x + 1.) *  width/2,   // Second, since the input models are scaled to have fit in the [-1,1]^3 world coordinates,
             (v.y + 1.) * height/2,
             v.z }; // why just v.z - because its not scaled to -1,1 anymore.
}
/*
    Depth interpolation - Z 'buffer
*/
static vec3 rot(vec3 v) {
    const double a = M_PI/6.0;
    const mat<double, 3,3> Ry = {{{std::cos(a), 0, std::sin(a)}, {0,1,0}, {-std::sin(a), 0, std::cos(a)}}};
    return Ry*v;
}


static void write_depth_image(const std::vector<float>& depth, int width, int height, const std::string& filename) {
    TGAImage image(width, height, TGAImage::GRAYSCALE);
    const auto [min_it, max_it] = std::minmax_element(depth.begin(), depth.end());
    const float min_depth = *min_it;
    const float max_depth = *max_it;
    const float scale = (max_depth > min_depth) ? 255.0f / (max_depth - min_depth) : 0.0f;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const float value = depth[x + y * width];
            if (!std::isfinite(value)) {
                image.set(x, y, {0});
                continue;
            }
            const unsigned char pixel = static_cast<unsigned char>(std::clamp((value - min_depth) * scale, 0.0f, 255.0f));
            image.set(x, y, {pixel});
        }
    }
    image.write_tga_file(filename);
}

// void draw(std::string filename, int width, int height, TGAImage& framebuffer, std::vector<double>& zbuffer, TGAColor color){
//     Model model(filename);
//     const auto& temp_points = model.get_points();
//     const auto& temp_faces = model.get_faces();
//     constexpr vec3    eye{-1,0,2}; // camera position
//     constexpr vec3 center{0,0,0};  // camera direction
//     constexpr vec3     up{0,1,0};  // camera up vector

//     lookat(eye, center, up);                              // build the ModelView   matrix
//     init_perspective(norm(eye-center));                        // build the Perspective matrix
//     init_viewport(width/16, height/16, width*7/8, height*7/8); // build the Viewport    matrix
//     RandomShader shader(model);

//     for(auto face : temp_faces) {
//         vec4 clip[3];
//         if (face[0] < 0 || face[0] >= static_cast<int>(temp_points.size()) ||
//             face[1] < 0 || face[1] >= static_cast<int>(temp_points.size()) ||
//             face[2] < 0 || face[2] >= static_cast<int>(temp_points.size())) {
//             std::cerr << "Indeks poza zakresem! Face: " << face[0] << ", " << face[1] << ", " << face[2] << std::endl;
//             continue;
//         }
//         shader.color = { std::rand()%255, std::rand()%255, std::rand()%255, 255 };
//         Triangle clip = {   shader.vertex(f, 0),  // assemble the primitive
//                             shader.vertex(f, 1),
//                             shader.vertex(f, 2) };


//         TGAColor rnd;
//         for (int c=0; c<3; c++) rnd[c] = std::rand()%255;
//         rasterize(clip, zbuffer, framebuffer, rnd, width, height, Viewport);
//     }
//     framebuffer.write_tga_file("framebuffer.tga");
// }