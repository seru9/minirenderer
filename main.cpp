#include <ctime>
#include "geometry.h"
#include "tgaimage.h"
#include "shapes_drawing_alghorithms.h"
#include "drawing_utils.h"
#include "model.h"

constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

constexpr int width  = 3000;
constexpr int height = 3000;

int main(int argc, char** argv) {
    TGAImage framebuffer(width, height, TGAImage::RGB);
    std::srand(std::time({}));
    draw("diablo3_pose.obj", width, height, framebuffer, white);

    return 0;
}