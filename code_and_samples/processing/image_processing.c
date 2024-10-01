#include <stdlib.h>
#include <stdio.h>
#include "../cbmp.h"
#include <limits.h>
#include <math.h>
#include <windows.h>
#include <psapi.h>
#include <time.h>
#include "pre_processing.c"
#include "../algorithms/convolution.c"
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
// unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char modified_image[BMP_WIDTH][BMP_HEIGTH];
unsigned char analysis_image[BMP_WIDTH][BMP_HEIGTH] = {0};
int kernel[KERNEL_SIZE][KERNEL_SIZE];
// unsigned char eroded_image[BMP_WIDTH][BMP_HEIGTH];

void printMemoryUsage()
{
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        printf("Peak Page File Bytes: %.2f MB\n", pmc.PeakPagefileUsage / (1024.0 * 1024.0));
        printf("Peak Working Set Bytes: %.2f MB\n", pmc.PeakWorkingSetSize / (1024.0 * 1024.0));
        printf("Page File Bytes: %.2f MB\n", pmc.PagefileUsage / (1024.0 * 1024.0));
        printf("Working Set Bytes: %.2f MB\n", pmc.WorkingSetSize / (1024.0 * 1024.0));
    }
}

void image_processing()
{
    unsigned char *input_ptr = &input_image[0][0][0];
    unsigned char *modified_ptr = &modified_image[0][0];
    unsigned char *analysis_ptr = &analysis_image[0][0];

    grey_scale(input_ptr, BMP_WIDTH, BMP_HEIGTH, BMP_CHANNELS, modified_ptr);

    binary_threshold(modified_ptr, BMP_WIDTH, BMP_HEIGTH);
    // convertTo3D(modified_image, output_image);
    // write_bitmap(output_image, "b_and_w.bmp");

    int *kernel_ptr = &kernel[0][0];

    create_kernel(kernel_ptr, KERNEL_SIZE, MAX_VALUE, MIN_VALUE);

    convolution(kernel_ptr, KERNEL_SIZE, modified_ptr, BMP_WIDTH, BMP_HEIGTH, analysis_ptr);

    /* convertTo3D(analysis_image, input_image);
    write_bitmap(input_image, "convulted_image1.bmp");
    convertTo3D(modified_image, input_image);
    write_bitmap(input_image, "modified_image1.bmp"); */
    
    // convertToUnsignedChar(analysis_image, modified_image);

    //analysis_loop(modified_image, analysis_image);
    analysis_loop(analysis_image, modified_image);
}

void testing(char *filepath)
{
    resetCoordinates();

    memset(analysis_image, 0, sizeof(analysis_image));

    read_bitmap(filepath, input_image);

    image_processing();

    // printf("Cells found: %d\n", coordinates_count);

    // printMemoryUsage();
}

void run(int argc, char **argv, int isTesting)
{
    if (argc != 3)
    {
        fprintf(stderr, "\nUsage: %s <output file path> <output file path>\n", argv[0]);
        exit(1);
    }

    if (isTesting)
    {
        testing(argv[1]);
        return;
    }

    clock_t start, end;
    start = clock();

    // argc counts how may arguments are passed
    // argv[0] is a string with the name of the program
    // argv[1] is the first command line argument (input image)
    // argv[2] is the second command line argument (output image)

    // Checking that 2 arguments are passed

    // printf("Example program - 02132 - A1\n");

    // Load image from file
    // Try reading the image until successful
    read_bitmap(argv[1], input_image);

    image_processing();

    draw_x(input_image);

    write_bitmap(input_image, argv[2]);

    printf("Cells found: %d\n", coordinates_count);

    //printMemoryUsage();

    end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_spent);
}
