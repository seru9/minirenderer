#pragma once
#include "tgaimage.h"

// shapes drawing
void line(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color);
void line2(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color);
void unoptimized_line(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color);
void optimized_line(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color);
void optimized_line2(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color);
void Bresenham(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color);
void Bresenham2(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color); 
void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer, TGAColor color);

// region drawing

void filled_triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer, TGAColor color);
void filled_triangle2(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, 
    TGAImage &framebuffer, TGAImage& zbuffer, TGAColor color);
void gamma_triangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, TGAImage &framebuffer, TGAImage& zbuffer);