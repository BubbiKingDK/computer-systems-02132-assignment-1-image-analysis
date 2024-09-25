// To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include <limits.h>
#include <math.h>
#include <windows.h>
#include <psapi.h>
#include <time.h>


#define THRESHOLD 90

#define LINE_LENGTH 10
#define THICKNESS 3

#define KERNEL_SIZE 25
#define RADIUS 12
#define MAX_VALUE 255
#define MIN_VALUE -140

int kernel[KERNEL_SIZE][KERNEL_SIZE];
int center = KERNEL_SIZE / 2;

unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char modified_image[BMP_WIDTH][BMP_HEIGTH];
int convoluted_image[BMP_WIDTH][BMP_HEIGTH] = {0};
unsigned char eroded_image[BMP_WIDTH][BMP_HEIGTH];

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
      black_white_image[x][y] = (grey_scale_image[x][y] >= THRESHOLD) ? 1 : 0;
    }
  }
}

void removeBackGround(unsigned char grey_scale_image[BMP_WIDTH][BMP_HEIGTH], unsigned char black_white_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      // black_white_image[x][y] = (grey_scale_image[x][y] >= THRESHOLD) ? black_white_image[x][y] : 0;
      black_white_image[x][y] = (grey_scale_image[x][y] >= THRESHOLD) ? 255 : 0;
    }
  }
}

void convertTo3D(unsigned char image[BMP_WIDTH][BMP_HEIGTH], unsigned char invertedImage[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  // int maxValue = findMaxValue(image);
  // maxValue = maxValue <= 0 ? 1 : maxValue;

  for (int i = 0; i < BMP_WIDTH; i++)
  {
    for (int j = 0; j < BMP_HEIGTH; j++)
    {
      invertedImage[i][j][0] = image[i][j];
      invertedImage[i][j][1] = image[i][j];
      invertedImage[i][j][2] = image[i][j];
    }
  }
}

void convertToUnsignedChar(int image[BMP_WIDTH][BMP_HEIGTH], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH])
{
  // int maxValue = findMaxValue(image);
  // maxValue = maxValue <= 0 ? 1 : maxValue;

  for (int i = 0; i < BMP_WIDTH; i++)
  {
    for (int j = 0; j < BMP_HEIGTH; j++)
    {
      output_image[i][j] = image[i][j];
    }
  }
}

unsigned char getPixelValue(unsigned char image[BMP_WIDTH][BMP_HEIGTH], int x, int y)
{
  if (x >= 0 && x < BMP_WIDTH && y >= 0 && y < BMP_HEIGTH)
  {
    return image[x][y];
  }
  return 0; // Out-of-bounds, return 0
}

void convolution(int kernel[KERNEL_SIZE][KERNEL_SIZE], unsigned char input[BMP_WIDTH][BMP_HEIGTH], int output[BMP_WIDTH][BMP_HEIGTH])
{
  unsigned char radius = KERNEL_SIZE / 2;
  int kernel_sum = 0;
  double temp = 0.037;

  // Beregn summen af kernelværdierne
  for (int i = 0; i < KERNEL_SIZE; i++)
  {
    for (int j = 0; j < KERNEL_SIZE; j++)
    {
      kernel_sum += kernel[i][j];
    }
  }

  for (int i = 0; i < BMP_WIDTH; i++)
  {
    for (int j = 0; j < BMP_HEIGTH; j++)
    {
      if (!getPixelValue(input, i, j))
      {
        continue;
      }
      int sum = 0;

      // Konvolutionsoperation
      for (int m = -radius; m <= radius; m++)
      {
        for (int n = -radius; n <= radius; n++)
        {
          // Hent pixelværdi med grænsekontrol
          unsigned char pixel_value = getPixelValue(input, i + m, j + n);
          sum += pixel_value * kernel[m + radius][n + radius];
        }
      }

      // Normaliser og begræns outputværdi
      if (kernel_sum != 0)
      {
        sum /= kernel_sum; // Normalisering af summen
      }

      int threshold = (int)(kernel_sum * temp);

      if ((i < 3 && i >= 0) || (i < BMP_WIDTH - 1 && i >= BMP_WIDTH - 4) || (j < 3 && j >= 0) || (j < BMP_HEIGTH - 1 && j >= BMP_HEIGTH - 4))
      {
        double temp = 0.7;
        threshold = (int)(threshold * temp);
      }

      if (sum >= threshold)
      {
        // Behold det område, der matcher kernel
        for (int m = -radius; m <= radius; m++)
        {
          for (int n = -radius; n <= radius; n++)
          {
            // Sørg for, at vi holder os inden for billedets grænser
            if (i + m >= 0 && i + m < BMP_WIDTH && j + n >= 0 && j + n < BMP_HEIGTH)
            {
              if (pow(m, 2) + pow(n, 2) < pow(radius / 5, 2))
              {
                output[i + m][j + n] = 255; // Sortér de omkringliggende pixels
              }
            }
          }
        }
      }
    }
  }
}

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

int erode_image(unsigned char current_image[BMP_WIDTH][BMP_HEIGTH], unsigned char next_image[BMP_WIDTH][BMP_HEIGTH])
{
  int eroded = 0;

  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if (current_image[x][y] != 255 || x - 1 < 0 || y - 1 < 0 || x + 1 >= BMP_WIDTH || y + 1 >= BMP_HEIGTH)
      {
        next_image[x][y] = 0;
        continue;
      }
      if (current_image[x - 1][y] && // Top
          current_image[x + 1][y] && // Bund
          current_image[x][y - 1] && // Venstre
          current_image[x][y + 1])   // Højre
      {
        next_image[x][y] = 255;
      }
      else
      {
        next_image[x][y] = 0;
        eroded = 1;
      }
    }
  }
  return eroded;
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

void analysis_loop(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH])
{
  unsigned char(*current_image)[BMP_HEIGTH] = input_image;
  unsigned char(*next_image)[BMP_HEIGTH] = output_image;

  int count = 0;
  int eroded = 1;

  while (eroded)
  {
    if (count % 2 == 0 || count == 0)
    {
      eroded = erode_image(current_image, next_image);
      if (!eroded)
      {
        break;
      }
      count_cells(next_image, current_image);
    }
    else
    {
      eroded = erode_image(next_image, current_image);
      if (!eroded)
      {
        break;
      }
      count_cells(current_image, next_image);
    }
    count++;
  }

  printf("Times eroded: %d\n", count);

  // Just for debugging
  if (count % 2 == 0 || count == 0)
  {
    for (int i = 0; i < BMP_WIDTH; i++)
    {
      for (int j = 0; j < BMP_HEIGTH; j++)
      {
        output_image[i][j] = input_image[i][j];
      }
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

void printMemoryUsage()
{
  PROCESS_MEMORY_COUNTERS pmc;
  if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
  {
    printf("Peak Page File Bytes: %zu\n", pmc.PeakPagefileUsage);
    printf("Peak Working Set Bytes: %zu\n", pmc.PeakWorkingSetSize);
    printf("Page File Bytes: %zu\n", pmc.PagefileUsage);
    printf("Working Set Bytes: %zu\n", pmc.WorkingSetSize);
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

  removeBackGround(modified_image, modified_image);
  convertTo3D(modified_image, output_image);
  write_bitmap(output_image, "b_and_w.bmp");

  create_kernel(kernel);

  convolution(kernel, modified_image, convoluted_image);

  convertToUnsignedChar(convoluted_image, modified_image);
  convertTo3D(modified_image, output_image);
  write_bitmap(output_image, "convulted_image.bmp");

  analysis_loop(modified_image, eroded_image);

  printf("Cells found: %d\n", coordinates_count);

  draw_x(input_image, output_image);

  write_bitmap(output_image, argv[2]);

  printMemoryUsage();

  free(coordinates);
  end = clock();
  double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Time taken: %f seconds\n", time_spent);

  return 0;
}
