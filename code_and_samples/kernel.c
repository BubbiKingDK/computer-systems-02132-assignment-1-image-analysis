#include <stdlib.h>
#include <stdio.h>

#define KERNEL_SIZE 25
#define RADIUS 12
#define MAX_VALUE 255
#define MIN_VALUE -140

int center = KERNEL_SIZE / 2;

int kernel[KERNEL_SIZE][KERNEL_SIZE];


void create_kernel(int kernel[KERNEL_SIZE][KERNEL_SIZE])
{
  for (int i = 0; i < KERNEL_SIZE; i++)
  {
    for (int j = 0; j < KERNEL_SIZE; j++)
    {
      // Compute the distance from the center
      double distance = sqrt((i - center) * (i - center) + (j - center) * (j - center));

      // Normalize the distance and calculate intensity
      if (distance <= RADIUS)
      {
        double normalized_distance = distance / RADIUS;
        kernel[i][j] = (int)(MAX_VALUE * (1 - normalized_distance));
      }
      else
      {
        kernel[i][j] = MIN_VALUE;
      }
    }
  }
}