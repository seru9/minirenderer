#include <cmath>
#include <cstdlib>
#include "line_drawing_alghorithms.h"
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
    if(ax > bx){ 
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    if(steep){
        std::swap(ax, ay);
        std::swap(bx, by);
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
    if(ax > bx){ 
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    if(steep){
        std::swap(ax, ay);
        std::swap(bx, by);
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
    if(ax > bx){ 
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    if(steep){
        std::swap(ax, ay);
        std::swap(bx, by);
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
    if(ax > bx){ 
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    if(steep){
        std::swap(ax, ay);
        std::swap(bx, by);
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
    if(ax > bx){ 
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    if(steep){
        std::swap(ax, ay);
        std::swap(bx, by);
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