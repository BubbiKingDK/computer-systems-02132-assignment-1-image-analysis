// This file contains methods for converting image to gray scale and applying binary threshold
#include <stdlib.h>
#include <stdio.h>

#define THRESHOLD 90

void grey_scale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char grey_scale_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      grey_scale_image[x][y] = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2] + 1) / 3;
    }
  }
}

void binary_threshold(unsigned char grey_scale_image[BMP_WIDTH][BMP_HEIGTH], unsigned char black_white_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      // black_white_image[x][y] = (grey_scale_image[x][y] >= THRESHOLD) ? black_white_image[x][y] : 0;
      black_white_image[x][y] = (grey_scale_image[x][y] >= THRESHOLD) ? 255 : 0;
    }
  }
}
