// This file contains methods for converting image to gray scale and applying binary threshold
#include <stdlib.h>
#include <stdio.h>

#define THRESHOLD 90

void grey_scale(unsigned char *input_ptr, int width, int height, int channels, unsigned char *grey_scale_ptr)
{
  for (int x = 0; x < width; x++)
  {
    for (int y = 0; y < height; y++)
    {
      int index = (x * height + y) * channels;
      unsigned char r = input_ptr[index];
      unsigned char g = input_ptr[index + 1];
      unsigned char b = input_ptr[index + 2];
      grey_scale_ptr[x * height + y] = (r + g + b) / 3;
    }
  }
}

/* #define MAX_INTENSITY 256

// Funktion til at beregne histogrammet af billedet
void calculate_histogram(unsigned char *image, int width, int height, int *histogram)
{
  for (int i = 0; i < MAX_INTENSITY; i++)
  {
    histogram[i] = 0;
  }

  // Gå igennem hver pixel og opdater histogrammet
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      histogram[image[i * height + j]]++;
    }
  }
}

// Otsu's metode til at beregne optimal tærskelværdi
int otsu_threshold(unsigned char *image, int width, int height)
{
  int histogram[MAX_INTENSITY];
  calculate_histogram(image, width, height, histogram);

  int total_pixels = width * height;
  int sum = 0, sum_b = 0, weight_b = 0, weight_f = 0;
  float max_variance = 0;
  int threshold = 0;

  // Beregn summen af alle intensiteter
  for (int i = 0; i < MAX_INTENSITY; i++)
  {
    sum += i * histogram[i];
  }

  // Gennemgå hver mulige tærskelværdi og find den, der maksimerer variansen
  for (int t = 0; t < MAX_INTENSITY; t++)
  {
    weight_b += histogram[t]; // Baggrundsvægt
    if (weight_b == 0)
      continue;

    weight_f = total_pixels - weight_b; // Forgrundsvægt
    if (weight_f == 0)
      break;

    sum_b += t * histogram[t];                      // Baggrundssum
    float mean_b = (float)sum_b / weight_b;         // Baggrundsgennemsnit
    float mean_f = (float)(sum - sum_b) / weight_f; // Forgrundsgennemsnit

    // Beregn variansen mellem klasserne
    float variance_between = (float)weight_b * weight_f * (mean_b - mean_f) * (mean_b - mean_f);

    // Find den maksimale varians og tilsvarende tærskelværdi
    if (variance_between > max_variance)
    {
      max_variance = variance_between;
      threshold = t;
    }
  }

  return threshold * 0.9;
}

// Funktion til at binarisere billedet ved brug af tærskelværdien
void apply_threshold(unsigned char *image, int width, int height, int threshold)
{
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (image[i * height + j] > threshold)
      {
        image[i * height + j] = 255; // Forgrund
      }
      else
      {
        image[i * height + j] = 0; // Baggrund
      }
    }
  }
}

void binary_threshold(unsigned char *grey_scale_ptr, int width, int height)
{
  int threshold = otsu_threshold(grey_scale_ptr, width, height);
  printf("Otsu's threshold: %d\n", threshold);

  apply_threshold(grey_scale_ptr, width, height, threshold);
} */

void binary_threshold(unsigned char *grey_scale_ptr, int width, int height)
{
  for (int x = 0; x < width; x++)
  {
    for (int y = 0; y < height; y++)
    {
      grey_scale_ptr[x * height + y] = (grey_scale_ptr[x * height + y] >= THRESHOLD) ? 255 : 0;
    }
  }
}
