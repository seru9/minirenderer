#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include "geometry.h"
#include "our_gl.h"
#include "model.h"
#include "tgaimage.h"
constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

struct PhongShader : IShader {
    const Model &model;
    TGAColor color = {};
    vec3 tri[3];  // triangle in eye coordinates
    vec3 light;
    vec3 varying_nrm[3];
    PhongShader(vec3 light, const Model &m) : model(m), light(light) {
    }

    virtual vec4 vertex(const int face, const int vert) {
        vec3 v = model.vert(face, vert);             // current vertex in object coordinates
        vec4 gl_Position = ModelView * vec4{v.x, v.y, v.z, 1.};
        vec3 n = model.normal(face, vert);
        varying_nrm[vert] = (ModelView.invert_transpose() * vec4{n.x, n.y, n.z, 0.}).xyz(); // don't understand
        tri[vert] = gl_Position.xyz();                            // in eye coordinates
        return Perspective * gl_Position;                         // in clip coordinates
    }

    virtual std::pair<bool,TGAColor> fragment(const vec3 bar) const {
        TGAColor col = {255, 255, 255, 255};
        vec3 A = tri[0];
        vec3 B = tri[1];
        vec3 C = tri[2];
        // vec3 n = normalized(cross(B - A, C - A));
        vec3 n = normalized(varying_nrm[0] * bar[0] +
                            varying_nrm[1] * bar[1] +
                            varying_nrm[2] * bar[2]);             // per-vertex normal interpolation
        vec3 r = normalized(n  * (n * light) * 2 - light);

        const double ambient = .3;
        const double diffuse = std::max(.0, n * light);
        const double specular = std::pow(std::max(r.z, 0.), 35);
        const double intensity = std::clamp(ambient + .4 * specular + .9 * diffuse, 0., 1.);

        for (int d : {0, 1, 2}) {
            col[d] = static_cast<uint8_t>(std::round(std::clamp(col[d] * intensity, 0., 255.)));
        }
        return {false, col};
    }
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " obj/model.obj" << std::endl;
        return 1;
    }

    constexpr int width  = 800;      // output image size
    constexpr int height = 800;
    constexpr vec3  light{ 1, 1, 1}; // light source
    constexpr vec3    eye{-1, 0, 2}; // camera position
    constexpr vec3 center{ 0, 0, 0}; // camera direction
    constexpr vec3     up{ 0, 1, 0}; // camera up vector
    lookat(eye, center, up);                                 // build the ModelView   matrix
    init_perspective(norm(eye-center));                        // build the Perspective matrix
    init_viewport(width/16, height/16, width*7/8, height*7/8); // build the Viewport    matrix
    init_zbuffer(width, height);
    TGAImage framebuffer(width, height, TGAImage::RGB);

    for (int m=1; m<argc; m++) {                    // iterate through all input objects
        Model model(argv[m]);                       // load the data
        PhongShader shader(light, model);
        for (int f=0; f<model.nfaces(); f++) {      // iterate through all facets
            Triangle clip = { shader.vertex(f, 0),  // assemble the primitive
                              shader.vertex(f, 1),
                              shader.vertex(f, 2) };
            rasterize(clip, shader, framebuffer);   // rasterize the primitive
        }
    }


    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}