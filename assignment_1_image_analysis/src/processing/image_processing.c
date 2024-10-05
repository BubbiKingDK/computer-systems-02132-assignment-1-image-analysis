#include <stdlib.h>
#include <stdio.h>
#include "../cbmp.h"
#include <limits.h>
#include <math.h>
#include <windows.h>
#include <psapi.h>
#include <time.h>
#include "pre_processing.c"
#include "../algorithms/modified_convolution.c"
#include "../helper_functions/kernel.c"
#include "../algorithms/erosion.c"
#include "../algorithms/detect_cells.h"
#include "../helper_functions/convert_array.c"
#include "post_processing.c"
#include "../helper_functions/pixel_value.h"
#include "../helper_functions/pixel_value.c"
#include "../analysis/analysis.c"

#define KERNEL_SIZE 25
#define MAX_VALUE 255
#define MIN_VALUE -140

int read_correct = 0;

unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char modified_image[BMP_WIDTH][BMP_HEIGTH];
unsigned char analysis_image[BMP_WIDTH][BMP_HEIGTH] = {0};
int kernel[KERNEL_SIZE][KERNEL_SIZE];

void image_processing()
{
    unsigned char *input_ptr = &input_image[0][0][0];
    unsigned char *modified_ptr = &modified_image[0][0];
    unsigned char *analysis_ptr = &analysis_image[0][0];

    // Perform grayscale conversion
    grey_scale(input_ptr, BMP_WIDTH, BMP_HEIGTH, BMP_CHANNELS, modified_ptr);

    // Apply binary thresholding
    binary_threshold(modified_ptr, BMP_WIDTH, BMP_HEIGTH);

    // Create convolution kernel
    int *kernel_ptr = &kernel[0][0];
    create_kernel(kernel_ptr, KERNEL_SIZE, MAX_VALUE, MIN_VALUE);

    // Perform convolution
    modified_convolution(kernel_ptr, KERNEL_SIZE, modified_ptr, BMP_WIDTH, BMP_HEIGTH, analysis_ptr);

    // Analysis processing
    analysis_loop(analysis_image, modified_image);
}

void testing(char *filepath)
{
    resetCoordinates();
    memset(analysis_image, 0, sizeof(analysis_image));
    read_bitmap(filepath, input_image);
    image_processing();
}

void run(int argc, char **argv, int isTesting)
{
    if (argc != 3)
    {
        fprintf(stderr, "\nUsage: %s <output file path> <output file path>\n", argv[0]);
        exit(1);
    }

    // Test mode
    if (isTesting)
    {
        testing(argv[1]);
        return;
    }

    // Read input image and process it
    read_bitmap(argv[1], input_image);
    image_processing();

    // Post-processing and output
    draw_x(input_image);
    write_bitmap(input_image, argv[2]);

    // Print analysis results
    printf("Cells found: %d\n", coordinates_count);
    //printMemoryUsage();
}
