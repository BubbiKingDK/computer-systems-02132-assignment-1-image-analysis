// gcc test.c -o test.exe -std=c99
// .\test.exe

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
            if(modified_image2[x][y] != (input_image2[x][y] >= treshhold ? 255 : 0)) {
                printf("Black and White test failed!\n");
                return;
            }
        }
    }
    printf("Black and White test succeeded!\n");
}

int testErosion(){
    // Define the folder and file names
    //Reading givin example:
    const char *input_folder = "results_example";
    const char *input_filename = "step_3.bmp";

    //Reading our image
    const char *sample_folder = "samples/easy";
    const char *sample_filename = "1EASY.bmp";

    //Writing image
    const char *output_folder = "results_example";
    const char *output_filename = "step_0_copy.bmp";



    
    // Construct the input and output file paths
    char input_filepath1[1024];
    snprintf(input_filepath1, sizeof(input_filepath1), "%s/%s", input_folder, input_filename);

    char input_filepath2[1024];
    snprintf(input_filepath2, sizeof(input_filepath2), "%s/%s", sample_folder, sample_filename);

    char output_filepath[1024];
    snprintf(output_filepath, sizeof(output_filepath), "%s/%s", output_folder, output_filename);

    // Print the file paths (for debugging purposes)
    printf("Reading givin BMP files: %s\n", input_filepath1);
    printf("Reading BMP file for erotion: %s\n", input_filepath2);
    printf("Writing BMP file: %s\n", output_filepath);

    // Prepare a buffer for the image data
    void *image_data = NULL;  // Allocate and initialize this as needed for read_bitmap
    read_bitmap(input_filepath1, input_image_ErosionMatch);
    read_bitmap(input_filepath2, input_image_testErosion);

   
    greyScale(input_image_testErosion, modified_greyScale);
    blackAndWhite(modified_greyScale, modified_greyScale);

    int temp = 0;
    while(temp < 3 && erodeImage(modified_greyScale, modified_greyScale)){
        temp++;
    }
    

    convertTo3D(modified_greyScale, input_image_testErosion);

    //write_bitmap(input_image_testErosion, output_filepath); 


    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            if (input_image_ErosionMatch[x][y][0] != input_image_testErosion[x][y][0] || input_image_ErosionMatch[x][y][1] != input_image_testErosion[x][y][1] || input_image_ErosionMatch[x][y][2] != input_image_testErosion[x][y][2]){
                return 0;
            } 
        }

    }
    return 1;
    
}

int main()
{
    testGrayScale();
    testBlackAndWhite();
    if (testErosion()) {
        printf("Erosion test succeeded!\n");
    } 
    else
    {
        printf("Erosion test failed!\n");
    }
    return 0;
}
