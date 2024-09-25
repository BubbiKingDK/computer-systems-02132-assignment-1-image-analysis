#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

void analysis_loop(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH])
{
  unsigned char(*current_image)[BMP_HEIGTH] = input_image;
  unsigned char(*next_image)[BMP_HEIGTH] = output_image;

  int count = 0;
  int eroded = 1;

  while (eroded)
  {
    if (count % 2 == 0 || count == 0)
    {
      eroded = erode_image(current_image, next_image);
      if (!eroded)
      {
        break;
      }
      count_cells(next_image, current_image);
    }
    else
    {
      eroded = erode_image(next_image, current_image);
      if (!eroded)
      {
        break;
      }
      count_cells(current_image, next_image);
    }
    count++;
  }

  printf("Times eroded: %d\n", count);

  // Just for debugging
  if (count % 2 == 0 || count == 0)
  {
    for (int i = 0; i < BMP_WIDTH; i++)
    {
      for (int j = 0; j < BMP_HEIGTH; j++)
      {
        output_image[i][j] = input_image[i][j];
      }
    }
  }
}


