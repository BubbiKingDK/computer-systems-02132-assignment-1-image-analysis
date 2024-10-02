#include <stdlib.h>
#include <stdio.h>
#include "../cbmp.h"

void analysis_loop(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH])
{

  unsigned char *current_ptr = &input_image[0][0];
  unsigned char *next_ptr = &output_image[0][0];

  int count = 0;
  int eroded = 1;

  while (eroded)
  {
    count++;
    eroded = erode_image(current_ptr, BMP_WIDTH, BMP_HEIGTH, next_ptr);
    if (!eroded)
    {
      break;
    }

    count_cells(next_ptr, BMP_WIDTH, BMP_HEIGTH);
    unsigned char *temp = current_ptr;
    current_ptr = next_ptr;
    next_ptr = temp;
  }
}
