#ifndef DETECT_CELLS_H
#define DETECT_CELLS_H
#include "../cbmp.h"
#define MAX_COORDINATES 500

int coordinates_count = 0;
typedef struct
{
  int x;
  int y;
} Coordinate;

Coordinate coordinates[MAX_COORDINATES];

void find_cell(unsigned char *current_ptr, int width, int height, int centerX, int centerY);
void count_cells(unsigned char *current_ptr, int width, int height);
void resetCoordinates();

#endif
