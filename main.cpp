#include <ctime>
#include "geometry.h"
#include "tgaimage.h"
#include "line_drawing_alghorithms.h"
#include "model.h"

constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

constexpr int width  = 800;
constexpr int height = 800;

std::tuple<int,int> project(double_vec3 v){ // First of all, (x,y) is an orthogonal projection of the vector (x,y,z).
    return { (v.x + 1.) *  width/2,   // Second, since the input models are scaled to have fit in the [-1,1]^3 world coordinates,
             (v.y + 1.) * height/2 }; // we want to shift the vector (x,y) and then scale it to span the entire screen.
}
int main(int argc, char** argv) {
    TGAImage framebuffer(width, height, TGAImage::RGB);
    std::srand(std::time({}));
    Model model("diablo3_pose.obj");
    const auto& temp_points = model.get_points();
    const auto& temp_faces = model.get_faces();
    for(auto point : temp_points){
        auto [ax, ay] = project(point);
        framebuffer.set(ax, ay, white);
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
        auto [ax, ay] = project(point1); 
        auto [bx, by] = project(point2);
        auto [cx, cy] = project(point3);
        Bresenham2(ax, ay, bx, by, framebuffer, red);
        Bresenham2(bx, by, cx, cy, framebuffer, red);
        Bresenham2(cx, cy, ax, ay, framebuffer, red);
        drawn_faces++;
    }
    framebuffer.write_tga_file("framebuffer.tga");
    std::cout << "Narysowano ścian: " << drawn_faces << " z " << temp_faces.size() << std::endl;
    return 0;
}