// This file contains method for erosion

#include <stdlib.h>
#include <stdio.h>

#include "../cbmp.h"

int erode_image(unsigned char *current_ptr, int width, int height, unsigned char *next_ptr)
{
  int eroded = 0;

  for (int x = 0; x < width; x++)
  {
    for (int y = 0; y < height; y++)
    {
      // Checking for pixels within bound
      if (current_ptr[x * height + y] != 255 || x - 1 < 0 || y - 1 < 0 || x + 1 >= width || y + 1 >= height)
      {
        next_ptr[x * height + y] = 0;
        continue;
      }
      // Checking surrounding pixels in a plus-pattern
      if (current_ptr[(x - 1) * height + y] &&
          current_ptr[(x + 1) * height + y] &&
          current_ptr[x * height + (y - 1)] &&
          current_ptr[x * height + (y + 1)])
      {
        next_ptr[x * height + y] = 255;
      }
      else
      {
        next_ptr[x * height + y] = 0;
        eroded = 1;
      }
    }
  }
  return eroded;
}
