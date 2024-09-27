// This file contains method for convolution
#include <stdlib.h>
#include <stdio.h>
#include "../cbmp.h"
#include "../helper_functions/pixel_value.h"

#define KERNEL_SIZE 25

void convolution(int kernel[KERNEL_SIZE][KERNEL_SIZE], unsigned char input[BMP_WIDTH][BMP_HEIGTH], int output[BMP_WIDTH][BMP_HEIGTH])
{
  unsigned char radius = KERNEL_SIZE / 2;
  int kernel_sum = 0;
  double temp = 0.037;

  // Beregn summen af kernelværdierne
  for (int i = 0; i < KERNEL_SIZE; i++)
  {
    for (int j = 0; j < KERNEL_SIZE; j++)
    {
      kernel_sum += kernel[i][j];
    }
  }

  for (int i = 0; i < BMP_WIDTH; i++)
  {
    for (int j = 0; j < BMP_HEIGTH; j++)
    {
      if (!getPixelValue(input, i, j))
      {
        continue;
      }

      if (output[i][j])
      {
        //continue;
      }

      int sum = 0;

      // Konvolutionsoperation
      for (int m = -radius; m <= radius; m++)
      {
        for (int n = -radius; n <= radius; n++)
        {
          // Hent pixelværdi med grænsekontrol
          unsigned char pixelvalue = getPixelValue(input, i + m, j + n);
          sum += pixelvalue * kernel[m + radius][n + radius];
        }
      }

      // Normaliser og begræns outputværdi
      if (kernel_sum != 0)
      {
        sum /= kernel_sum; // Normalisering af summen
      }

      int threshold = (int)(kernel_sum * temp);

      if ((i < 3 && i >= 0) || (i < BMP_WIDTH - 1 && i >= BMP_WIDTH - 4) || (j < 3 && j >= 0) || (j < BMP_HEIGTH - 1 && j >= BMP_HEIGTH - 4))
      {
        double temp = 0.7;
        threshold = (int)(threshold * temp);
      }

      if (sum >= threshold)
      {
        // Behold det område, der matcher kernel
        for (int m = -2; m <= 2; m++)
        {
          for (int n = -2; n <= 2; n++)
          {
            // Sørg for, at vi holder os inden for billedets grænser
            if (i + m >= 0 && i + m < BMP_WIDTH && j + n >= 0 && j + n < BMP_HEIGTH)
            {
              if (pow(m, 2) + pow(n, 2) < pow(radius / 4, 2))
              {
                output[i + m][j + n] = 255;
              }
            }
          }
        }
      }
    }
  }
}