#include <stdlib.h>
#include <stdio.h>
#include "../cbmp.h"
#include <limits.h>
#include <math.h>
#include <windows.h>
#include <psapi.h>
#include <time.h>
#include "../processing/pre_processing.c"
#include "../algorithms/convolution.c"
#include "../algorithms/erosion.c"
#include "../helper_functions/convert_array.c"
#include "../helper_functions/pixel_value.c"
#include "../algorithms/detect_cells.c"
#include "../algorithms/detect_cells.h"

#define WIDTH 950
#define treshold 90

// unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
// unsigned char modified_image[BMP_WIDTH][BMP_HEIGTH];

// unsigned char input_image2[BMP_WIDTH][BMP_HEIGTH];
// unsigned char modified_image2[BMP_WIDTH][BMP_HEIGTH];

/* unsigned char input_image3[BMP_WIDTH][BMP_HEIGTH];

unsigned char modified_image3[BMP_WIDTH][BMP_HEIGTH];

unsigned char input_image_testErosion[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char input_image_ErosionMatch[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char modified_greyScale[BMP_WIDTH][BMP_HEIGTH];
unsigned char modified_eroded_image[BMP_WIDTH][BMP_HEIGTH]; */

void testGrayScale()
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
            if (grey_scale_ptr[i * 5 + j] != (100 + 150 + 200) / 3)
            {
                printf("GrayScale test failed!\n");
                return;
            }
        }
    }
    printf("GrayScale test succeeded!\n");
}
/* void testGrayScale()
{
    for (int i = 0; i < BMP_WIDTH; i++)
    {
        for (int j = 0; j < BMP_HEIGTH; j++)
        {
            input_image[i][j][0] = 100;
            input_image[i][j][1] = 150;
            input_image[i][j][2] = 200;
        }
    }
    grey_scale(input_image, modified_image);
    for (int i = 0; i < BMP_WIDTH; i++)
    {
        for (int j = 0; j < BMP_HEIGTH; j++)
        {
            unsigned char expected = (100 + 150 + 200) / 3;
            if (modified_image[i][j] != expected)
            {
                printf("GrayScale test failed!\n");
                return;
            }
        }
    }
    printf("GrayScale test succeeded!\n");
} */

void testBlackAndWhite()
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
                    printf("Binary threshold test failed!\n");
                    return;
                }
            }
            else
            {
                if (image_ptr[x * 5 + y] != 0)
                {
                    printf("Binary threshold test failed!\n");
                    return;
                }
            }
        }
    }
    printf("Binary threshold test succeeded!\n");
}
/*
void applyGreyScaleAndBlackAndWhite(int *a)
{
    grey_scale(input_image_testErosion, modified_greyScale);
    binary_threshold(modified_greyScale, modified_greyScale);
    *a = 0;
}
*/

void testErosion()
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
                printf("Erosion test failed!\n");
                return;
            }
        }
    }
    printf("Erosion test succeeded!\n");
}

void testFindCell()
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
                printf("Find cells test step 1 failed!\n");
                return;
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
                printf("Find cells test step 2 failed!\n");
                return;
            }
        }
    }
    printf("Find cells test succeeded!\n");
}

/*
int testErosion()
{
    // Define the folder and file names
    const char *input_folder = "results_example";
    const char *input_filenames[] = {
        "step_1.bmp",
        "step_2.bmp",
        "step_3.bmp",
        "step_4.bmp",
        "step_5.bmp",
        "step_6.bmp",
        "step_7.bmp",
        "step_8.bmp",
        "step_9.bmp"};

    const char *output_folder = "results_example";
    const char *output_filename = "step_0_copy.bmp";

    const char *sample_folder = "results_example";
    const char *sample_filename = "input_image.bmp";

    // Construct the input and output file paths

    char input_filepath2[1024];
    snprintf(input_filepath2, sizeof(input_filepath2), "%s/%s", sample_folder, sample_filename);

    char output_filepath[1024];
    snprintf(output_filepath, sizeof(output_filepath), "%s/%s", output_folder, output_filename);

    // Print the file paths (for debugging purposes)
    // printf("Reading BMP file: %s\n", input_filepath1);
    // printf("Writing BMP file: %s\n", output_filepath);

    // Prepare a buffer for the image data
    void *image_data = NULL; // Allocate and initialize this as needed for read_bitmap
    read_bitmap(input_filepath2, input_image_testErosion);

    // Prepare the output image data (you might modify or process the image_data here)
    // For demonstration, we'll use the same data for output
    // (In practice, you would modify image_data as needed)

    // Write the image data to the new BMP file
    // write_bitmap(input_image_ErosionMatch, output_filepath);

    for (int i = 0; i < 4; i++)
    {
        char input_filepath1[1024];
        snprintf(input_filepath1, sizeof(input_filepath1), "%s/%s", input_folder, input_filenames[i]);

        read_bitmap(input_filepath1, input_image_ErosionMatch);

        int a = 1;
        if (a)
        {
            applyGreyScaleAndBlackAndWhite(&a);
        }

        convertTo3D(modified_greyScale, input_image_testErosion);

        erode_image(modified_greyScale, modified_eroded_image);

        convertTo3D(modified_eroded_image, input_image_testErosion);

        write_bitmap(input_image_testErosion, output_filepath);

        int match = 1;
        for (int x = 0; x < BMP_WIDTH; x++)
        {
            for (int y = 0; y < BMP_HEIGTH; y++)
            {
                if (input_image_ErosionMatch[x][y][0] != input_image_testErosion[x][y][0] || input_image_ErosionMatch[x][y][1] != input_image_testErosion[x][y][1] || input_image_ErosionMatch[x][y][2] != input_image_testErosion[x][y][2])
                {
                    match = 0;
                    break;
                }
            }
            if (!match)
            {
                break;
            }
        }
        if (!match)
        {
            printf("Erosion test failed at step %d (file: %s)!\n", i + 1, input_filenames[i]);
            return 0;
        }
    }
    printf("Erosion test succeeded!\n");
    return 1;
}
 */

int main()
{
    testGrayScale();
    testBlackAndWhite();
    testErosion();
    testFindCell();

    return 0;
}