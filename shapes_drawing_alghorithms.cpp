#include <algorithm>
#include <cmath>
#include <cstdlib>
#include "shapes_drawing_alghorithms.h"
#include "tgaimage.h"
void line(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color)
{
    for(float t = 0.0; t < 1.0; t += 0.02){
        int xt = std::round(ax + (bx - ax) * t);
        int yt = std::round(ay + (by - ay) * t);
        framebuffer.set(xt, yt, color);
    }
    
}
void line2(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color)
{
    for(int xt = ax; xt <= bx; xt++){
        float t = (xt - ax) / static_cast<float>((bx - ax));
        int yt = std::round(ay + (by - ay) * t);
        framebuffer.set(xt, yt, color);
    }
    
}
void unoptimized_line(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color)
{
    bool steep = std::abs(ax - bx) < std::abs(ay - by);
    if(steep){
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if(ax > bx){ 
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    for(int xt = ax; xt <= bx; xt++){
        float t = (xt - ax) / static_cast<float>((bx - ax));
        int yt = std::round(ay + (by - ay) * t);
        if(steep){
            framebuffer.set(yt, xt, color); //de-transpose
        }else{
            framebuffer.set(xt, yt, color);
        }
    }
    
}
void optimized_line(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color){
    bool steep = std::abs(ax - bx) < std::abs(ay - by);
    if(steep){
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if(ax > bx){ 
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    int yt = ay;
    for(int xt = ax; xt <= bx; xt++){
        yt += (by - ay) / static_cast<float>(bx - ax);
        if(steep){
            framebuffer.set(yt, xt, color); //de-transpose
        }else{
            framebuffer.set(xt, yt, color);
        }
    }
    
}
void optimized_line2(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color){
    bool steep = std::abs(ax - bx) < std::abs(ay - by);
    float error;
    if(steep){
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if(ax > bx){ 
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    int yt = ay;
    for(int xt = ax; xt <= bx; xt++){
        if(steep){
            framebuffer.set(yt, xt, color); //de-transpose
        }else{
            framebuffer.set(xt, yt, color);
        }
        error = std::abs(by - ay) / static_cast<float>(bx - ax);
        if(error >.5){
            yt += by > ay ? 1 : -1;
            error -= 1.;
        }
    }
    
}
void Bresenham(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color){
    bool steep = std::abs(ax - bx) < std::abs(ay - by);
    int ierror = 0;
    if(steep){
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if(ax > bx){ 
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    int yt = ay;
    for(int xt = ax; xt <= bx; xt++){
        if(steep){
            framebuffer.set(yt, xt, color); //de-transpose
        }else{
            framebuffer.set(xt, yt, color);
        }
        ierror += 2 * std::abs(by - ay);
        if(ierror >bx - ax){ // performance killer, not to be estimated constantly calculated Bresenham2 changes it
            yt += by > ay ? 1 : -1;
            ierror -= 2 * (bx - ax);
        }
    }
}
void Bresenham2(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color){
    bool steep = std::abs(ax - bx) < std::abs(ay - by);
    int ierror = 0;
    if(steep){
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if(ax > bx){ 
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    int yt = ay;
    for(int xt = ax; xt <= bx; xt++){
        if(steep){
            framebuffer.set(yt, xt, color); //de-transpose
        }else{
            framebuffer.set(xt, yt, color);
        }
        ierror += 2 * std::abs(by - ay);
        yt += (by > ay ? 1 : -1) * (ierror > bx - ax); //big change (not if)
        ierror -= 2 * (bx-ax)   * (ierror > bx - ax);
    }
}
void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer, TGAColor color){
    Bresenham2(ax, ay, bx, by, framebuffer, color);
    Bresenham2(bx, by, cx, cy, framebuffer, color);
    Bresenham2(cx, cy, ax, ay, framebuffer, color);
}
// old fashioned single-thread approach
void filled_triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer, TGAColor color){
    if (ay>by) { std::swap(ax, bx); std::swap(ay, by); }
    if (ay>cy) { std::swap(ax, cx); std::swap(ay, cy); }
    if (by>cy) { std::swap(bx, cx); std::swap(by, cy); }
    int total_height = cy - ay;
    if(by != ay){
        int segment_height = by - ay;
        for(int y = ay; y <= by; y++){
            int x1 = ax + ((cx - ax) * (y - ay)) / total_height;
            int x2 = ax + ((bx - ax) * (y - ay)) / segment_height;
            for(int x = std::min(x1, x2); x < std::max(x1, x2); x++){
                framebuffer.set(x, y, color);
            }
        }
    }
    if(cy != by){
        int segment_height = cy - by;
        for(int y = by; y <= cy; y++){
            int x1 = ax + ((cx - ax) * (y - by)) / total_height;
            int x2 = ax + ((cx - bx) * (y - by)) / segment_height;
            for(int x = std::min(x1, x2); x < std::max(x1, x2); x++){
                framebuffer.set(x, y, color);
            }
        }
    }
}
// different approach with bouding box
/*  
Here we'll use Barycentric coordinates any point P can be shown via combination of points:
A, B, C so P = alphaA + betaB + gammaC 
alpha, beta and gamma are equivalent to the sub-triangle areas PBC, PCA, PAB
*/
static double signed_triangle_area(int ax, int ay, int bx, int by, int cx, int cy) {
    return .5*((by-ay)*(bx+ax) + (cy-by)*(cx+bx) + (ay-cy)*(ax+cx));
}

void filled_triangle2(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, 
    TGAImage &framebuffer, TGAImage& zbuffer, TGAColor color) {
    int bbminx = std::min(std::min(ax, bx), cx); // bounding box for the triangle
    int bbminy = std::min(std::min(ay, by), cy); // defined by its top left and bottom right corners
    int bbmaxx = std::max(std::max(ax, bx), cx);
    int bbmaxy = std::max(std::max(ay, by), cy);
    double total_area = signed_triangle_area(ax, ay, bx, by, cx, cy);
    if(total_area < 1)
        return;
    #pragma omp parallel for
    for (int x=bbminx; x<=bbmaxx; x++) {
        for (int y=bbminy; y<=bbmaxy; y++) {
            double alpha = signed_triangle_area(x, y, bx, by, cx, cy) / total_area;
            double beta  = signed_triangle_area(x, y, cx, cy, ax, ay) / total_area;
            double gamma = signed_triangle_area(x, y, ax, ay, bx, by) / total_area;
            unsigned char z = static_cast<unsigned char>(alpha * az + beta * bz + gamma * cz);
            if (alpha<0 || beta<0 || gamma<0) continue; // negative barycentric coordinate => the pixel is outside the triangle
            if (z <= zbuffer.get(x, y)[0]) continue; // z-buffer hidden surfaces removal
            zbuffer.set(x, y, {z});
            framebuffer.set(x, y, color);
        }
    }
}
// gamma_triangle 
// coordinate z is the color coordinate
void gamma_triangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, 
    TGAImage &framebuffer, TGAImage& zbuffer){
    int bbminx = std::min(std::min(ax, bx), cx); // bounding box for the triangle
    int bbminy = std::min(std::min(ay, by), cy); // defined by its top left and bottom right corners
    int bbmaxx = std::max(std::max(ax, bx), cx);
    int bbmaxy = std::max(std::max(ay, by), cy);
    double total_area = signed_triangle_area(ax, ay, bx, by, cx, cy);
    if(total_area < 1)
        return;
    #pragma omp parallel for
    for (int x=bbminx; x<=bbmaxx; x++) {
        for (int y=bbminy; y<=bbmaxy; y++) {
            double alpha = signed_triangle_area(x, y, bx, by, cx, cy) / total_area;
            double beta  = signed_triangle_area(x, y, cx, cy, ax, ay) / total_area;
            double gamma = signed_triangle_area(x, y, ax, ay, bx, by) / total_area;
            unsigned char r = static_cast<unsigned char> (alpha * 255);
            unsigned char g = static_cast<unsigned char> (beta * 255);
            unsigned char b = static_cast<unsigned char> (gamma * 255);
            if (alpha<0 || beta<0 || gamma<0) continue; // negative barycentric coordinate => the pixel is outside the triangle
            framebuffer.set(x, y, {r, g, b});
        }
    }
}
