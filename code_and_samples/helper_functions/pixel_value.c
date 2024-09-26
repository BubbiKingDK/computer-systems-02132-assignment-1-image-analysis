#include <stdlib.h>
#include <stdio.h>
#include "../cbmp.h"
#include "pixel_value.h"


unsigned char getPixelValue(unsigned char image[BMP_WIDTH][BMP_HEIGTH], int x, int y)
{
  if (x >= 0 && x < BMP_WIDTH && y >= 0 && y < BMP_HEIGTH)
  {
    return image[x][y];
  }
  return 0; // Out-of-bounds, return 0
}