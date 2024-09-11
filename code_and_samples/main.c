// To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

#define treshhold 90

/*
//Function to invert pixels of an image (negative)
void invert(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      int average = 0;
      for (int c = 0; c < BMP_CHANNELS; c++)
      {
        output_image[x][y][c] = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2]) / 3;
      }
    }
  }
}
*/

void greyScale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char grey_scale_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      // Calculate the grayscale value only once per pixel
      int average = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2] + 1) / 3;

      for (int c = 0; c < BMP_CHANNELS; c++)
      {
        grey_scale_image[x][y][c] = average;
      }
    }
  }
}

void blackAndWhite(unsigned char grey_scale_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if (grey_scale_image[x][y][0] >= treshhold)
      {
        for (int c = 0; c < BMP_CHANNELS; c++)
        {
          output_image[x][y][c] = 255;
        }
      }
      else
      {
        for (int c = 0; c < BMP_CHANNELS; c++)
        {
          output_image[x][y][c] = 0;
        }
      }
    }
  }
}

// Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char grey_scale_image[BMP_WIDTH][BMP_HEIGTH];

// Main function
int main(int argc, char **argv)
{
  // argc counts how may arguments are passed
  // argv[0] is a string with the name of the program
  // argv[1] is the first command line argument (input image)
  // argv[2] is the second command line argument (output image)

  // Checking that 2 arguments are passed
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
    exit(1);
  }

  printf("Example program - 02132 - A1\n");

  // Load image from file
  read_bitmap(argv[1], input_image);

  // Run inversion
  greyScale(input_image, grey_scale_image);
  blackAndWhite(grey_scale_image, output_image);

  // Save image to file
  write_bitmap(output_image, argv[2]);

  printf("Done!\n");
  return 0;
}
