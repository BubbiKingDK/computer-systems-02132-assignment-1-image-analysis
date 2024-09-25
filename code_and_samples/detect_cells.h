#ifndef DETECT_CELLS_H
#define DETECT_CELLS_H
#include "cbmp.h"

int coordinates_count = 0;
typedef struct
{
  int x;
  int y;
} Coordinate;

Coordinate *coordinates = NULL;

void find_cell(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH], int centerX, int centerY);
void count_cells(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH]);

#endif