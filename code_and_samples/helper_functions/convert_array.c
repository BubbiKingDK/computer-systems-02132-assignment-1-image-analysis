#include <stdlib.h>
#include <stdio.h>
#include "../cbmp.h"

void convertTo3D(unsigned char image[BMP_WIDTH][BMP_HEIGTH], unsigned char invertedImage[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  for (int i = 0; i < BMP_WIDTH; i++)
  {
    for (int j = 0; j < BMP_HEIGTH; j++)
    {
      invertedImage[i][j][0] = image[i][j];
      invertedImage[i][j][1] = image[i][j];
      invertedImage[i][j][2] = image[i][j];
    }
  }
}

void convertToUnsignedChar(int image[BMP_WIDTH][BMP_HEIGTH], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int i = 0; i < BMP_WIDTH; i++)
  {
    for (int j = 0; j < BMP_HEIGTH; j++)
    {
      output_image[i][j] = image[i][j];
    }
  }
}
