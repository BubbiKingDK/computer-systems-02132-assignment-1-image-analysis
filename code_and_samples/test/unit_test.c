#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <windows.h>
#include <psapi.h>
#include <time.h>
#include "../src/processing/pre_processing.c"
#include "../src/algorithms/convolution.c"
#include "../src/algorithms/erosion.c"
#include "../src/helper_functions/convert_array.c"
#include "../src/helper_functions/pixel_value.c"
#include "../src/helper_functions/kernel.c"
#include "../src/algorithms/detect_cells.c"
#include "../src/algorithms/detect_cells.h"
#include "unit_test_styling.c"

#define WIDTH 950
#define treshold 90

int read_correct;

int testGrayScale(char *failure_message)
{
    unsigned char input_image[5][5][3];
    unsigned char modified_image[5][5];

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            input_image[i][j][0] = 100;
            input_image[i][j][1] = 150;
            input_image[i][j][2] = 200;
        }
    }
    unsigned char *input_ptr = &input_image[0][0][0];
    unsigned char *grey_scale_ptr = &modified_image[0][0];
    grey_scale(input_ptr, 5, 5, 3, grey_scale_ptr);

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (grey_scale_ptr[i * 5 + j] != (100 + 150 + 200) * 86 / 256)
            {
                sprintf(failure_message, "Expected 150, but was %d", grey_scale_ptr[i * 5 + j]);
                return 0;
            }
        }
    }
    return 1;
}

int testBlackAndWhite(char *failure_message)
{
    unsigned char image[5][5];
    unsigned char *image_ptr = &image[0][0];

    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 5; y++)
        {
            if (x % 2 == 0)
            {
                image_ptr[x * 5 + y] = 100;
            }
            else
            {
                image_ptr[x * 5 + y] = 80;
            }
        }
    }

    binary_threshold(image_ptr, 5, 5);

    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 5; y++)
        {
            if (x % 2 == 0)
            {
                if (image_ptr[x * 5 + y] != 255)
                {
                    sprintf(failure_message, "Expected 255, but was %d", image_ptr[x * 5 + y]);
                    return 0;
                }
            }
            else
            {
                if (image_ptr[x * 5 + y] != 0)
                {
                    sprintf(failure_message, "Expected 0, but was %d", image_ptr[x * 5 + y]);
                    return 0;
                }
            }
        }
    }
    return 1;
}

int testErosion(char *failure_message)
{
    unsigned char input_image[5][5] = {
        {0, 0, 255, 0, 0},
        {0, 255, 255, 255, 0},
        {255, 255, 255, 255, 255},
        {0, 255, 255, 255, 0},
        {0, 0, 255, 0, 0}};

    unsigned char modified_image[5][5];

    unsigned char *input_ptr = &input_image[0][0];
    unsigned char *modified_ptr = &modified_image[0][0];

    erode_image(input_ptr, 5, 5, modified_ptr);

    unsigned char expected_image[5][5] = {
        {0, 0, 0, 0, 0},
        {0, 0, 255, 0, 0},
        {0, 255, 255, 255, 0},
        {0, 0, 255, 0, 0},
        {0, 0, 0, 0, 0}};

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (modified_ptr[i * 5 + j] != expected_image[i][j])
            {
                sprintf(failure_message, "Expected %d, but was %d", expected_image[i][j], modified_ptr[i * 5 + j]);
                return 0;
            }
        }
    }
    return 1;
}

int testFindCell(char *failure_message)
{
    unsigned char input_image[5][5] = {
        {0, 0, 255, 0, 0},
        {0, 255, 255, 255, 0},
        {255, 255, 255, 255, 255},
        {0, 255, 255, 255, 0},
        {0, 0, 255, 0, 0}};

    unsigned char *input_ptr = &input_image[0][0];

    count_cells(input_ptr, 5, 5);

    unsigned char expected_image[5][5] = {0};

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (input_ptr[i * 5 + j] != expected_image[i][j])
            {
                sprintf(failure_message, "Expected %d, but was %d", expected_image[i][j], input_ptr[i * 5 + j]);
                return 0;
            }
        }
    }

    unsigned char input_image_2[13][13];
    unsigned char expected_image_2[13][13];
    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            input_image_2[i][j] = 255;
            expected_image_2[i][j] = 255;
        }
    }

    unsigned char *input_ptr_2 = &input_image_2[0][0];

    count_cells(input_ptr_2, 13, 13);

    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            if (input_ptr_2[i * 13 + j] != expected_image_2[i][j])
            {
                sprintf(failure_message, "Expected %d, but was %d", expected_image[i][j], input_ptr[i * 13 + j]);
                return 0;
            }
        }
    }
    return 1;
}

int testConvolution(char *failure_message)
{
    int size = 5;
    int max = 2;
    int min = -5;

    int kernel[size][size];
    int *kernel_ptr = &kernel[0][0];

    unsigned char image1[5][5] = {
        {255, 255, 0, 255, 255},
        {255, 0, 0, 0, 255},
        {0, 0, 255, 0, 0},
        {255, 0, 0, 0, 255},
        {255, 255, 0, 255, 255}};

    unsigned char *image_ptr1 = &image1[0][0];

    unsigned char image2[5][5] = {
        {0, 0, 0, 0, 0},
        {0, 0, 255, 0, 0},
        {0, 255, 255, 255, 0},
        {0, 0, 255, 0, 0},
        {0, 0, 0, 0, 0}};

    unsigned char *image_ptr2 = &image2[0][0];

    unsigned char output1[5][5] = {0};
    unsigned char *output_ptr1 = &output1[0][0];

    unsigned char output2[5][5] = {0};
    unsigned char *output_ptr2 = &output2[0][0];

    unsigned char expected1[5][5] = {0};

    unsigned char expected2[5][5] = {
        {0, 255, 255, 255, 0},
        {0, 255, 255, 255, 0},
        {0, 255, 255, 255, 0},
        {0, 255, 255, 255, 0},
        {0, 255, 255, 255, 0}};

    create_kernel(kernel_ptr, size, max, min);

    convolution(kernel_ptr, size, image_ptr1, 5, 5, output_ptr1);

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (output_ptr1[i * 5 + j] != expected1[i][j])
            {
                sprintf(failure_message, "Expected %d, but was %d", expected1[i][j], output_ptr1[i * 5 + j]);
                return 0;
            }
        }
    }

    convolution(kernel_ptr, size, image_ptr2, 5, 5, output_ptr2);

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (output_ptr2[i * 5 + j] != expected2[i][j])
            {
                sprintf(failure_message, "Expecteded %d, but was %d", expected2[i][j], output_ptr2[i * 5 + j]);
                return 0;
            }
        }
    }
    return 1;
}

int main()
{
    TestBlueprint blueprint = {0, 0, 0, {""}, {""}}; // Initialiser testsuite

    int total_tests = 5; // Angiv antal tests

    char failure_message[256]; // Buffer til fejlmeddelelse

    // Kør tests
    printf("\n");
    run_test(&blueprint, "Grey Scale Test", testGrayScale, failure_message, total_tests);
    run_test(&blueprint, "Binary Treshold Test", testBlackAndWhite, failure_message, total_tests);
    run_test(&blueprint, "Erosion Test", testErosion, failure_message, total_tests);
    run_test(&blueprint, "Find Cell Test", testFindCell, failure_message, total_tests);
    run_test(&blueprint, "Convolution Test", testConvolution, failure_message, total_tests);

    // Udskriv afsluttende progress bar
    print_progress_bar(total_tests, blueprint.test_passed + blueprint.test_failed, blueprint.test_passed, blueprint.test_failed);

    if (blueprint.test_failed == 0)
    {
        printf(GREEN "\n\nAll tests passed successfully!" RESET);
    }
    else
    {
        printf(RED "\n\nSome tests failed." RESET);
    }

    print_test_results(blueprint);

    return 0;
}
