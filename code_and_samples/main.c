// To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

//gcc cbmp.c main.c -o main.exe -std=c99 -lpsapi -O3

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include <limits.h>
#include <math.h>
#include <windows.h>
#include <psapi.h>
#include <time.h>
#include "pre_processing.c"
#include "convolution.c"
#include "kernel.c"
#include "erosion.c"
#include "detect_cells.h"
#include "convert_array.c"
#include "after_processing.c"
#include "pixel_value.h"
#include "pixel_value.c"



unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char modified_image[BMP_WIDTH][BMP_HEIGTH];
int convoluted_image[BMP_WIDTH][BMP_HEIGTH] = {0};
unsigned char eroded_image[BMP_WIDTH][BMP_HEIGTH];






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

  grey_scale(input_image, modified_image);

  binary_threshold(modified_image, modified_image);
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
