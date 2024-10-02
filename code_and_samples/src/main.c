// To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

// gcc cbmp.c main.c -o main.exe -std=c99 -lpsapi -O3

#include <stdlib.h>
#include <stdio.h>
#include "processing/image_processing.c"

// Main function
int main(int argc, char **argv)
{
  run(argc, argv, 0);

  return 0;
}
