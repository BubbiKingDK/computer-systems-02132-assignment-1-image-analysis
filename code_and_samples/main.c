// To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

#include <time.h>

#define treshhold 90

#define LINE_LENGTH 10
#define THICKNESS 3

int count = 1;
int coordinates_count = 0;

typedef struct
{
  int x;
  int y;
} Coordinate;

Coordinate *coordinates = NULL;

void addCoordinate(int x, int y)
{
  coordinates_count++;
  coordinates = (Coordinate *)realloc(coordinates, coordinates_count * sizeof(Coordinate));

  if (coordinates == NULL)
  {
    fprintf(stderr, "Memory allocation failed!\n");
    exit(1);
  }

  coordinates[coordinates_count - 1].x = x;
  coordinates[coordinates_count - 1].y = y;
}

// Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char modified_image[BMP_WIDTH][BMP_HEIGTH];

unsigned char distance_image[BMP_WIDTH][BMP_HEIGTH];
unsigned char eroded_image[BMP_WIDTH][BMP_HEIGTH];

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
  // Moduler!
  int offsets[4][2] = {
      {-1, 0}, // Left
      {1, 0},  // Right
      {0, -1}, // Up
      {0, 1}   // Down
  };

  for (int i = 0; i < sizeof(offsets) / sizeof(offsets[0]); i++)
  {

    int new_x = x + offsets[i][0];
    int new_y = y + offsets[i][1];

    // Check if surounding pixels is in bound:
    if (new_x >= 0 && new_x < BMP_WIDTH && new_y >= 0 && new_y < BMP_HEIGTH)
    {
      if (image[new_x][new_y] != 255)
      {
        return 0;
      }
    }
    else
    {
      // Might need an edge case handling here...:
      return 0;
    }
  }
  return 1;
}

/*
int whiteInOuter(int window[13][13])
{
  for (int i = 0; i < 13; i++)
  {
    if (window[0][i] || window[12][i] || window[i][0] || window[i][12])
    {
      return 1;
    }
  }
  return 0;
}

int checkInner(int window[13][13], int centerX, int centerY)
{
  int sum_x = 0, sum_y = 0, count = 0;

  for (int i = 0; i < 13; i++)
  {
    for (int j = 0; j < 13; j++)
    {
      if (window[i][j] == 1)
      {
        sum_x += i;
        sum_y += j;
        count++;
      }
    }
  }

  if (count == 0)
  {
    return 0;
  }

  addCoordinate(centerX + sum_x / count - 6, centerY + sum_y / count - 6);
  return 1;
}
*/

unsigned char getPixelValue(unsigned char image[BMP_WIDTH][BMP_HEIGTH], int x, int y)
{
  if (x >= 0 && x < BMP_WIDTH && y >= 0 && y < BMP_HEIGTH)
  {
    return image[x][y];
  }
  return 0; // Out-of-bounds, return 0
}

void extract_window(unsigned char image[BMP_WIDTH][BMP_HEIGTH], int centerX, int centerY, unsigned char modified_image[BMP_WIDTH][BMP_HEIGTH])
{
  if (!image[centerX][centerY])
  {
    return;
  }

  int radius = 6;
  for (int i = -radius; i <= radius; i++)
  {
    if (getPixelValue(image, centerX - radius, centerY + i) || getPixelValue(image, centerX + radius, centerY + i) || getPixelValue(image, centerX + i, centerY - radius) || getPixelValue(image, centerX + i, centerY + radius))
    {
      return;
    }
  }

  addCoordinate(centerX, centerY);

  // If a dot is found, make the surrounding area black
  for (int i = -radius; i <= radius; i++)
  {
    for (int j = -radius; j <= radius; j++)
    {
      int x = centerX + i;
      int y = centerY + j;

      if (x >= 0 && x < BMP_WIDTH && y >= 0 && y < BMP_HEIGTH)
      {
        modified_image[x][y] = 0;
      }
    }
  }
}

int min(int a, int b)
{
  return (a < b) ? a : b;
}

void distance(unsigned char black_white_image[BMP_WIDTH][BMP_HEIGTH], unsigned char distance_image[BMP_WIDTH][BMP_HEIGTH])
{
  // Initialize the distance_image array:
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if (black_white_image[x][y] == 0)
      {
        distance_image[x][y] = 0; // Black point, distance = 0
      }
      else
      {
        distance_image[x][y] = 255; // White point, initial distance = "infinite" (255)
      }
    }
  }

  // Forward pass
  for (int i = 0; i < BMP_HEIGTH; i++)
  {
    for (int j = 0; j < BMP_WIDTH; j++)
    {
      if (i > 0)
        distance_image[i][j] = min(distance_image[i][j], distance_image[i - 1][j] + 1);
      if (j > 0)
        distance_image[i][j] = min(distance_image[i][j], distance_image[i][j - 1] + 1);
    }
  }

  // Backward pass
  for (int i = BMP_HEIGTH - 1; i >= 0; i--)
  {
    for (int j = BMP_WIDTH - 1; j >= 0; j--)
    {
      if (i < BMP_HEIGTH - 1)
        distance_image[i][j] = min(distance_image[i][j], distance_image[i + 1][j] + 1);
      if (j < BMP_WIDTH - 1)
        distance_image[i][j] = min(distance_image[i][j], distance_image[i][j + 1] + 1);
    }
  }

  for (int i = 0; i < BMP_HEIGTH; i++)
  {
    for (int j = 0; j < BMP_WIDTH; j++)
    {
      distance_image[i][j] = distance_image[i][j] < 3 ? 0 : 255;
    }
  }
}

void countDots()
{
  for (int i = 0; i < BMP_WIDTH; i++)
  {
    for (int j = 0; j < BMP_HEIGTH; j++)
    {
      extract_window(modified_image, i, j, modified_image);
    }
  }
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
      if (temp_image[x][y] != 255)
      {
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
  for (int i = 0; i < BMP_WIDTH; i++) {
    for (int j = 0; j < BMP_HEIGTH; j++) {
      eroded_image[i][j] = black_white_image[i][j];
    }
  }
  // printf("Eroded has run %d times\n", count++);
  if (eroded)
  {
    countDots();
  }

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
  //distance(modified_image, modified_image);

  while (erodeImage(modified_image, eroded_image));

  /*for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
          printf("%d ", window[i][j]);
      }
      printf("\n");
  }*/

  // Debugging
  // Convert grayscale 2D image to 3D RGB format

  //convertTo3D(modified_image, output_image);

  // Save image to file
  printf("We found %d dots!\n", coordinates_count);
  printf("Done!\n");

  draw_x(input_image, output_image);

  write_bitmap(output_image, argv[2]);

  free(coordinates);
  end = clock();
  double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Time taken: %f seconds\n", time_spent);

  return 0;
}
