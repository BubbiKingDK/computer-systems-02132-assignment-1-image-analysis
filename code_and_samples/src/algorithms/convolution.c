// This file contains method for convolution
#include <stdlib.h>
#include <stdio.h>
#include "../helper_functions/pixel_value.h"

void convolution(int *kernel, int kernel_size, unsigned char *input, int width, int height, unsigned char *output)
{
  unsigned char radius = kernel_size / 2;
  int kernel_sum = 0;
  double temp = 0.037;

  // Beregn summen af kernelværdierne
  for (int i = 0; i < kernel_size; i++)
  {
    for (int j = 0; j < kernel_size; j++)
    {
      kernel_sum += kernel[i * kernel_size + j];
    }
  }

  for (int i = 0; i < width; i++)
  {
    for (int j = 0; j < height; j++)
    {
      if (!getPixelValue(input, width, height, i, j))
      {
        continue;
      }

      if (output[i * height + j])
      {
        continue;
      }

      int sum = 0;

      // Konvolutionsoperation
      for (int m = -radius; m <= radius; m++)
      {
        for (int n = -radius; n <= radius; n++)
        {
          // Hent pixelværdi med grænsekontrol
          unsigned char pixelvalue = getPixelValue(input, width, height, i + m, j + n);
          sum += pixelvalue * kernel[(m + radius) * kernel_size + (n + radius)];
        }
      }

      // Normaliser og begræns outputværdi
      if (kernel_sum != 0)
      {
        sum /= abs(kernel_sum);
      }

      int threshold = (int)(kernel_sum * temp);

      if ((i < 4 && i >= 0) || (i < width - 1 && i >= height - 5) || (j < 4 && j >= 0) || (j < height - 1 && j >= width - 5))
      {
        double temp = 0.7;
        threshold = (int)(threshold * temp);
      }

      if (sum >= threshold)
      {
        // Behold det område, der matcher kernel
        for (int m = -1; m <= 1; m++)
        {
          for (int n = -1; n <= 1; n++)
          {
            // Sørg for, at vi holder os inden for billedets grænser
            if (i + m >= 0 && i + m < width && j + n >= 0 && j + n < height)
            {
              output[(i + m) * height + (j + n)] = 255;
            }
          }
        }
      }
    }
  }
}
