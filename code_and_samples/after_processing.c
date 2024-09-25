#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include "detect_cells.c"

#define LINE_LENGTH 10
#define THICKNESS 3

void draw_x(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      for (int c = 0; c < BMP_CHANNELS; c++)
      {
        output_image[x][y][c] = input_image[x][y][c];
      }
    }
  }

  for (int i = 0; i < coordinates_count; i++)
  {
    int cx = coordinates[i].x;
    int cy = coordinates[i].y;

    for (int j = -LINE_LENGTH; j <= LINE_LENGTH; j++)
    {
      for (int t = -THICKNESS / 2; t <= THICKNESS / 2; t++)
      {
        int y = cy + j;
        int x = cx + t;

        if (x >= 0 && x < BMP_WIDTH && y >= 0 && y < BMP_HEIGTH)
        {
          output_image[x][y][0] = 255;
          output_image[x][y][1] = 0;
          output_image[x][y][2] = 0;
        }
      }
    }

    for (int j = -LINE_LENGTH; j <= LINE_LENGTH; j++)
    {
      for (int t = -THICKNESS / 2; t <= THICKNESS / 2; t++)
      {
        int x = cx + j;
        int y = cy + t;

        if (x >= 0 && x < BMP_WIDTH && y >= 0 && y < BMP_HEIGTH)
        {
          output_image[x][y][0] = 255;
          output_image[x][y][1] = 0;
          output_image[x][y][2] = 0;
        }
      }
    }
  }
}


