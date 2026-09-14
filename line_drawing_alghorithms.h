#pragma once
#include "tgaimage.h"

void line(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color);
void line2(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color);
void unoptimized_line(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color);
void optimized_line(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color);
void optimized_line2(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color);
void Bresenham(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color);
void Bresenham2(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color); 