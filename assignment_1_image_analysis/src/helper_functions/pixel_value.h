#ifndef PIXEL_VALUE_H
#define PIXEL_VALUE_H
#include "../cbmp.h"

unsigned char getPixelValue(unsigned char *image_ptr,int width,int height, int x, int y);
unsigned char getPixelValue2(unsigned char image[BMP_WIDTH][BMP_HEIGTH],int x, int y);

#endif
