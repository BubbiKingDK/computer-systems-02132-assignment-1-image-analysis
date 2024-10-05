#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <psapi.h>
#include "../src/processing/image_processing.c"

#define TOTAL_CELLS 300

extern int read_correct;

// Terminal output template
void run_test(const char *folder, const char **input_filenames, int file_count, const char *output_folder)
{
    clock_t start, end;
    double total_time = 0.0;
    int total_cells = 0, 
    pic_count = file_count;

    printf("\n---------------------------\n");
    printf("         %s Test         \n", folder);
    printf("---------------------------\n");
    printf("%-10s %-10s %-10s\n", "Test", "Cells", "Time (s)");
    printf("---------------------------\n");

    for (int i = 0; i < file_count; i++)
    {
        read_correct = 1;
        char input_filepath[1024];
        snprintf(input_filepath, sizeof(input_filepath), "samples/%s/%s", folder, input_filenames[i]);

        char *argv[] = {"", input_filepath, ""};
        int argc = sizeof(argv) / sizeof(argv[0]);

        start = clock();
        run(argc, argv, 1);   
        draw_x(input_image); 
        char output_filepath[1024];
        snprintf(output_filepath, sizeof(output_filepath), "test/output/%s/%s%d.bmp", output_folder, folder, i + 1);
        write_bitmap(input_image, output_filepath); 
        end = clock();

        if (read_correct)
        {
            double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
            printf("%-10d %-10d %-10.3f\n", i + 1, coordinates_count, cpu_time_used);
            total_time += cpu_time_used;
        }
        else
        {
            printf("%-10d %-10d %-10s\n", i + 1, coordinates_count, "error");
            pic_count--;
        }
        total_cells += coordinates_count;
    }

    // Summary
    int average_cells = total_cells / file_count;
    double percentage_found = ((double)average_cells / TOTAL_CELLS) * 100;
    double average_time = total_time / pic_count;

    printf("---------------------------\n");
    printf("Average time: %.3f s\n", average_time);
    printf("Average found cells: %d\n", average_cells);
    printf("Percentage of total cells: %.1f%%\n", percentage_found);
    printf("---------------------------\n");
}

void test_easy()
{
    const char *input_filenames_easy[10] = {
        "1EASY.bmp", "2EASY.bmp", "3EASY.bmp", "4EASY.bmp", "5EASY.bmp", 
        "6EASY.bmp", "7EASY.bmp", "8EASY.bmp", "9EASY.bmp", "10EASY.bmp"};
    
    run_test("easy", input_filenames_easy, 10, "easy");
}

void test_medium()
{
    const char *input_filenames_medium[10] = {
        "1MEDIUM.bmp", "2MEDIUM.bmp", "3MEDIUM.bmp", "4MEDIUM.bmp", "5MEDIUM.bmp",
        "6MEDIUM.bmp", "7MEDIUM.bmp", "8MEDIUM.bmp", "9MEDIUM.bmp", "10MEDIUM.bmp"};

    run_test("medium", input_filenames_medium, 10, "medium");
}

void test_hard()
{
    const char *input_filenames_hard[10] = {
        "1HARD.bmp", "2HARD.bmp", "3HARD.bmp", "4HARD.bmp", "5HARD.bmp",
        "6HARD.bmp", "7HARD.bmp", "8HARD.bmp", "9HARD.bmp", "10HARD.bmp"};

    run_test("hard", input_filenames_hard, 10, "hard");
}

void test_impossible()
{
    const char *input_filenames_impossible[5] = {
        "1IMPOSSIBLE.bmp", "2IMPOSSIBLE.bmp", "3IMPOSSIBLE.bmp", "4IMPOSSIBLE.bmp", "5IMPOSSIBLE.bmp"};

    run_test("impossible", input_filenames_impossible, 5, "impossible");
}

int main()
{
    test_easy();
    test_medium();
    test_hard();
    test_impossible();

    return 0;
}
