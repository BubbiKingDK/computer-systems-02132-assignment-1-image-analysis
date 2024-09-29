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

void find_cell(unsigned char *current_ptr, int width, int height, int centerX, int centerY)
{
  if (!current_ptr[centerX * height + centerY])
  {
    return;
  }

  int radius = 6;
  for (int i = -radius; i <= radius; i++)
  {
    if (getPixelValue(current_ptr, width, height, centerX - radius, centerY + i) || getPixelValue(current_ptr, width, height, centerX + radius, centerY + i) || getPixelValue(current_ptr, width, height, centerX + i, centerY - radius) || getPixelValue(current_ptr, width, height, centerX + i, centerY + radius))
    {
      return;
    }
  }

  addCoordinate(centerX, centerY);
  //printf("(%d, %d)", centerX, centerY);

  for (int i = -radius; i <= radius; i++)
  {
    for (int j = -radius; j <= radius; j++)
    {
      int x = centerX + i;
      int y = centerY + j;

      if (x >= 0 && x < width && y >= 0 && y < height)
      {
        current_ptr[x * height + y] = 0;
      }
    }
  }
}

void count_cells(unsigned char *current_ptr, int width, int height)
{
  for (int i = 0; i < width; i++)
  {
    for (int j = 0; j < height; j++)
    {
      find_cell(current_ptr, width, height, i, j);
    }
  }
  /* unsigned char *temp = current_ptr;
  current_ptr = next_ptr;
  next_ptr = temp; */
}
