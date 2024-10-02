#include <stdlib.h>
#include <stdio.h>
#include "../cbmp.h"
#include "pixel_value.h"

unsigned char getPixelValue(unsigned char *image_ptr,int width,int height, int x, int y)
{
  if (x >= 0 && x < width && y >= 0 && y < height)
  {
    return image_ptr[x * height + y];
  }
  return 0; // Out-of-bounds, return 0
}

unsigned char getPixelValue2(unsigned char image[BMP_WIDTH][BMP_HEIGTH],int x, int y)
{
  if (x >= 0 && x < BMP_WIDTH && y >= 0 && y < BMP_HEIGTH)
  {
    return image[x][y];
  }
  return 0; // Out-of-bounds, return 0
}
