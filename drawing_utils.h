#pragma once
#include "tgaimage.h"
#include "shapes_drawing_alghorithms.h"
#include "geometry.h"
#include "model.h"
#include "our_gl.h"
void draw(std::string filename, int width, int height, TGAImage& framebuffer,std::vector<double>& zbuffer, TGAColor color);
