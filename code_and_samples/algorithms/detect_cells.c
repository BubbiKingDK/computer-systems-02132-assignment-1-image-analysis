#include <stdlib.h>
#include <stdio.h>
#include "../cbmp.h"
#include "../helper_functions/pixel_value.h"

#include "detect_cells.h"

void addCoordinate(int x, int y)
{
  if (coordinates_count >= MAX_COORDINATES)
  {
    fprintf(stderr, "Cannot add more coordinates: limit reached!\n");
    return; // Stop, hvis vi overskrider grænsen
  }

  coordinates[coordinates_count].x = x;
  coordinates[coordinates_count].y = y;
  coordinates_count++;
}

void resetCoordinates()
{
  coordinates_count = 0;
}

void find_cell(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH], int centerX, int centerY)
{
  if (!input_image[centerX][centerY])
  {
    return;
  }

  int radius = 6;
  for (int i = -radius; i <= radius; i++)
  {
    if (getPixelValue(input_image, centerX - radius, centerY + i) || getPixelValue(input_image, centerX + radius, centerY + i) || getPixelValue(input_image, centerX + i, centerY - radius) || getPixelValue(input_image, centerX + i, centerY + radius))
    {
      return;
    }
  }

  addCoordinate(centerX, centerY);
  // printf("Found cell at (%d, %d)\n", centerX, centerY);

  for (int i = -radius; i <= radius; i++)
  {
    for (int j = -radius; j <= radius; j++)
    {
      int x = centerX + i;
      int y = centerY + j;

      if (x >= 0 && x < BMP_WIDTH && y >= 0 && y < BMP_HEIGTH)
      {
        input_image[x][y] = 0;
      }
    }
  }
}

void count_cells(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int i = 0; i < BMP_WIDTH; i++)
  {
    for (int j = 0; j < BMP_HEIGTH; j++)
    {
      find_cell(input_image, i, j);
    }
  }
  unsigned char(*temp)[BMP_HEIGTH] = input_image;
  input_image = output_image;
  output_image = temp;
}