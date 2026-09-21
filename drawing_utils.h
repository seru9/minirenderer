#pragma once
#include "tgaimage.h"
#include "shapes_drawing_alghorithms.h"
#include "geometry.h"
#include "model.h"
void draw(std::string filename, int width, int height, TGAImage& framebuffer,TGAImage& zbuffer, TGAColor color);