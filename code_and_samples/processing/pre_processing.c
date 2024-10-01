// This file contains methods for converting image to gray scale and applying binary threshold
#include <stdlib.h>
#include <stdio.h>

#define THRESHOLD 90

void grey_scale(unsigned char *input_ptr, int width, int height, int channels, unsigned char *grey_scale_ptr)
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            int index = (x * height + y) * channels;
            unsigned char r = input_ptr[index];
            unsigned char g = input_ptr[index + 1];
            unsigned char b = input_ptr[index + 2];
            grey_scale_ptr[x * height + y] = (r + g + b) / 3;
        }
    }
}

void binary_threshold(unsigned char *grey_scale_ptr,int width,int height)
{
  for (int x = 0; x <width; x++)
  {
    for (int y = 0; y < height; y++)
    {
      grey_scale_ptr[x * height + y] = (grey_scale_ptr[x * height + y]  >= THRESHOLD) ? 255 : 0;
    }
  }
}
