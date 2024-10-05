#include <stdlib.h>
#include <stdio.h>

//creats the kernel used for convolution
void create_kernel(int *kernel_ptr, int kernel_size, int max_value, int min_value)
{
  int radius = kernel_size / 2;

  for (int i = 0; i < kernel_size; i++)
  {
    for (int j = 0; j < kernel_size; j++)
    {
      // Compute the distance from the center
      double distance = sqrt((i - radius) * (i - radius) + (j - radius) * (j - radius));

      // Normalize the distance and calculate intensity
      if (distance <= radius)
      {
        double normalized_distance = distance / radius;
        kernel_ptr[i * kernel_size + j] = (int)(max_value * (1 - normalized_distance));
      }
      else
      {
        kernel_ptr[i * kernel_size + j] = min_value;
      }
    }
  }
}
