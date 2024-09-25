#include <stdlib.h>
#include <stdio.h>

#include "cbmp.h"

int erode_image(unsigned char current_image[BMP_WIDTH][BMP_HEIGTH], unsigned char next_image[BMP_WIDTH][BMP_HEIGTH])
{
  int eroded = 0;

  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if (current_image[x][y] != 255 || x - 1 < 0 || y - 1 < 0 || x + 1 >= BMP_WIDTH || y + 1 >= BMP_HEIGTH)
      {
        next_image[x][y] = 0;
        continue;
      }
      if (current_image[x - 1][y] && // Top
          current_image[x + 1][y] && // Bund
          current_image[x][y - 1] && // Venstre
          current_image[x][y + 1])   // Højre
      {
        next_image[x][y] = 255;
      }
      else
      {
        next_image[x][y] = 0;
        eroded = 1;
      }
    }
  }
  return eroded;
}