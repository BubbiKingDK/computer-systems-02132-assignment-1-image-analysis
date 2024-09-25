#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include "cbmp.c"
#include <string.h>

#include "main.c"

#define WIDTH 950

unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char modified_image[BMP_WIDTH][BMP_HEIGTH];

unsigned char input_image2[BMP_WIDTH][BMP_HEIGTH];
unsigned char modified_image2[BMP_WIDTH][BMP_HEIGTH];

unsigned char input_image3[BMP_WIDTH][BMP_HEIGTH];

unsigned char modified_image3[BMP_WIDTH][BMP_HEIGTH];

unsigned char input_image_testErosion[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char input_image_ErosionMatch[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char modified_greyScale[BMP_WIDTH][BMP_HEIGTH];

void testGrayScale()
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
    greyScale(input_image, modified_image);
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
}

void testBlackAndWhite()
{
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            input_image2[x][y] = 100;
        }
    }
    blackAndWhite(input_image2, modified_image2);
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            if (modified_image2[x][y] != (input_image2[x][y] >= treshhold ? 255 : 0))
            {
                printf("Black and White test failed!\n");
                return;
            }
        }
    }
    printf("Black and White test succeeded!\n");
}

void applyGreyScaleAndBlackAndWhite(int *a)
{
    greyScale(input_image_testErosion, modified_greyScale);
    blackAndWhite(modified_greyScale, modified_greyScale);
    *a = 0;
}

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

        erodeImage(modified_greyScale, modified_greyScale);

        convertTo3D(modified_greyScale, input_image_testErosion);

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
            if(!match) {
                break;
            }
        }
        if (!match) {
            printf("Erosion test failed at step %d (file: %s)!\n", i+1, input_filenames[i]);
            return 0;
        }
    }
    printf("Erosion test succeeded!\n");
    return 1;
}

int main()
{
    testGrayScale();
    testBlackAndWhite();
    testErosion();
}
