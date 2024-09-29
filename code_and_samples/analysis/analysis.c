#include <stdlib.h>
#include <stdio.h>
#include "../cbmp.h"

unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

void analysis_loop(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH])
{
  // unsigned char(*current_image)[BMP_HEIGTH] = input_image;
  // unsigned char(*next_image)[BMP_HEIGTH] = output_image;

  unsigned char *current_ptr = &input_image[0][0];
  unsigned char *next_ptr = &output_image[0][0];

  int count = 0;
  int eroded = 1;

  /* char path[1024];
  snprintf(path, sizeof(path), "analysis/test_erotion/step%d.bmp", count);

  for (int i = 0; i < BMP_WIDTH; i++)
  {
    for (int j = 0; j < BMP_HEIGTH; j++)
    {
      image[i][j][0] = current_ptr[i * BMP_WIDTH + j]; // Red channel
      image[i][j][1] = current_ptr[i * BMP_WIDTH + j]; // Green channel
      image[i][j][2] = current_ptr[i * BMP_WIDTH + j]; // Blue channel
    }
  }
  write_bitmap(image, path); */

  while (eroded)
  {
    count++;
    eroded = erode_image(current_ptr, BMP_WIDTH, BMP_HEIGTH, next_ptr);
    if (!eroded)
    {
      break;
    }
    // printf("%d: ", count++);
    count_cells(next_ptr, BMP_WIDTH, BMP_HEIGTH);
    unsigned char *temp = current_ptr;
    current_ptr = next_ptr;
    next_ptr = temp;
    // printf("\n");

    /* char path[1024];
    snprintf(path, sizeof(path), "analysis/test_erotion/step%d.bmp", count);

    for (int i = 0; i < BMP_WIDTH; i++)
    {
      for (int j = 0; j < BMP_HEIGTH; j++)
      {
        image[i][j][0] = current_ptr[i * BMP_WIDTH + j]; // Red channel
        image[i][j][1] = current_ptr[i * BMP_WIDTH + j]; // Green channel
        image[i][j][2] = current_ptr[i * BMP_WIDTH + j]; // Blue channel
      }
    }
    write_bitmap(image, path); */

    /* if (count % 2 == 0 || count == 0)
    {
      eroded = erode_image(current_ptr, BMP_WIDTH, BMP_HEIGTH, next_ptr);
      if (!eroded)
      {
        break;
      }
      count_cells(next_ptr, BMP_WIDTH, BMP_HEIGTH, current_ptr);
    }
    else
    {
      eroded = erode_image(next_ptr, BMP_WIDTH, BMP_HEIGTH, current_ptr);
      if (!eroded)
      {
        break;
      }
      count_cells(current_ptr, BMP_WIDTH, BMP_HEIGTH, next_ptr);
    }
    count++; */
  }

  // printf("Times eroded: %d\n", count);

  // Just for debugging
  /* if (count % 2 == 0 || count == 0)
  {
    for (int i = 0; i < BMP_WIDTH; i++)
    {
      for (int j = 0; j < BMP_HEIGTH; j++)
      {
        output_image[i][j] = input_image[i][j];
      }
    }
  } */
}
