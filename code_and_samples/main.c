// To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

#include <time.h>


#define treshhold 90

int count = 1;

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

/*
// Old Grey scale:
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
*/

// New Grey Scale function:
void greyScale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char grey_scale_image[BMP_WIDTH][BMP_HEIGTH])
{
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            grey_scale_image[x][y] = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2] + 1) / 3;
        }
    }
}

void blackAndWhite(unsigned char grey_scale_image[BMP_WIDTH][BMP_HEIGTH], unsigned char black_white_image[BMP_WIDTH][BMP_HEIGTH])
{
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            black_white_image[x][y] = (grey_scale_image[x][y] >= treshhold) ? 255 : 0;
        }
    }
}

// Function to check if a pixel and its surrounding pixels are white
int checkSurroundingPixels(int x, int y, unsigned char image[BMP_WIDTH][BMP_HEIGTH])
{
    //Moduler!
    int offsets[4][2] = {
        {-1, 0},  // Left
        {1, 0},   // Right
        {0, -1},  // Up
        {0, 1}    // Down
    };

    for (int i = 0; i < sizeof(offsets)/sizeof(offsets[0]) ; i++) {

        int new_x = x + offsets[i][0];
        int new_y = y + offsets[i][1];

        // Check if surounding pixels is in bound:
        if (new_x >= 0 && new_x < BMP_WIDTH && new_y >= 0 && new_y < BMP_HEIGTH) {
            if (image[new_x][new_y] != 255) {
                return 0;
            }
        }
        else 
        {
            //Might need an edge case handling here...:
            return 0;
        }
    }
    return 1;
}

int erodeImage(unsigned char black_white_image[BMP_WIDTH][BMP_HEIGTH], unsigned char eroded_image[BMP_WIDTH][BMP_HEIGTH])
{
    int eroded = 0;
    unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH];
    
    // Copy image to temp:
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            temp_image[x][y] = black_white_image[x][y];
        }
    }

    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
          if (temp_image[x][y] != 255) {
            black_white_image[x][y] = 0;
            continue;
          }
          if (!checkSurroundingPixels(x, y, temp_image))  
          {
              black_white_image[x][y] = 0;
              eroded = 1; 
          }
        }
    }
    eroded_image = black_white_image;
    printf("Eroded has run %d times\n", count++);
    return eroded;
    
}

// Debugging
//  This function converts the 2D image back into a 3D image so we can svae the image to a file, through the provided functions.
void convertTo3D(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
      for (int y = 0; y < BMP_HEIGTH; y++)
      {
          unsigned char pixel_value = input_image[x][y];
          output_image[x][y][0] = pixel_value; 
          output_image[x][y][1] = pixel_value;
          output_image[x][y][2] = pixel_value;  
      }
  }
}

// Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char modified_image[BMP_WIDTH][BMP_HEIGTH];

// Main function
int main(int argc, char **argv)
{
  clock_t start, end;
  start = clock();

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

  greyScale(input_image, modified_image);
  blackAndWhite(modified_image, modified_image);

  /*
  for (int i = 0; i < 26; i++) {
    erodeImage(modified_image, modified_image);
  }
  */
  


  while(erodeImage(modified_image, modified_image));



  
  // Debugging
  // Convert grayscale 2D image to 3D RGB format
  convertTo3D(modified_image, output_image);

  // Save image to file
  write_bitmap(output_image, argv[2]);

  printf("Done!\n");
  
  
  end = clock();
  double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Time taken: %f seconds\n", time_spent);

  return 0;
}
