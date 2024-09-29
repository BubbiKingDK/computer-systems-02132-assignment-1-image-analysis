#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <psapi.h>
#include "../processing/image_processing.c"

#define TOTAL_CELLS 300

extern int read_correct;

void test_easy()
{
    int pic_count = 10;
    const char *sample_folder = "samples";
    const char *easy_folder = "easy";
    const char *input_filenames_easy[10] = {
        "1EASY.bmp",
        "2EASY.bmp",
        "3EASY.bmp",
        "4EASY.bmp",
        "5EASY.bmp",
        "6EASY.bmp",
        "7EASY.bmp",
        "8EASY.bmp",
        "9EASY.bmp",
        "10EASY.bmp"};

    clock_t start, end;
    double cpu_time_used;
    int total_cells = 0;
    double total_time = 0.0;

    printf("\n---------------------------\n");
    printf("          Easy Test       \n");
    printf("---------------------------\n");
    printf("%-10s %-10s %-10s\n", "Test", "Cells", "Time (s)");
    printf("---------------------------\n");

    for (int i = 0; i < 10; i++)
    {
        read_correct = 1;
        char input_filepath_easy[1024];
        snprintf(input_filepath_easy, sizeof(input_filepath_easy), "%s/%s/%s", sample_folder, easy_folder, input_filenames_easy[i]);

        char *argv[] = {
            "",
            ("%s", input_filepath_easy),
            ""};
        int argc = sizeof(argv) / sizeof(argv[0]);

        // For debugging purposes, you can print the filepath:
        // printf("File path: %s\n", input_filepath_easy);

        start = clock();
        run(argc, argv, 1);
        draw_x(input_image);
        char path[1024]; // Tildel en passende størrelse til 'path'
        snprintf(path, sizeof(path), "test/output/easy/easy%d.bmp", i + 1);
        write_bitmap(input_image, path);
        end = clock();

        if (read_correct)
        {
            cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
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
    int average_cells = total_cells / 10;
    double percentage_found = ((double)average_cells / TOTAL_CELLS) * 100;
    double average_time = (double)(total_time / pic_count);

    printf("---------------------------\n");
    printf("Average time %.3f s\n", average_time);
    printf("Average found cells: %d\n", average_cells);
    printf("Percentage of total cells: %.1f%%\n", percentage_found);
    printf("---------------------------\n");
}

void test_medium()
{
    int pic_count = 10;
    const char *sample_folder = "samples";
    const char *medium_folder = "medium";
    const char *input_filenames_medium[10] = {
        "1MEDIUM.bmp",
        "2MEDIUM.bmp",
        "3MEDIUM.bmp",
        "4MEDIUM.bmp",
        "5MEDIUM.bmp",
        "6MEDIUM.bmp",
        "7MEDIUM.bmp",
        "8MEDIUM.bmp",
        "9MEDIUM.bmp",
        "10MEDIUM.bmp"};

    clock_t start, end;
    double cpu_time_used;
    int total_cells = 0;
    double total_time = 0.0;

    printf("\n---------------------------\n");
    printf("        Medium Test        \n");
    printf("---------------------------\n");
    printf("%-10s %-10s %-10s\n", "Test", "Cells", "Time (s)");
    printf("---------------------------\n");

    for (int i = 0; i < 10; i++)
    {
        read_correct = 1;
        char input_filepath_medium[1024];
        snprintf(input_filepath_medium, sizeof(input_filepath_medium), "%s/%s/%s", sample_folder, medium_folder, input_filenames_medium[i]);

        char *argv[] = {
            "",
            ("%s", input_filepath_medium),
            ""};
        int argc = sizeof(argv) / sizeof(argv[0]);

        // For debugging purposes, you can print the filepath:
        // printf("File path: %s\n", input_filepath_easy);

        start = clock();
        run(argc, argv, 1);
        draw_x(input_image);
        char path[1024]; // Tildel en passende størrelse til 'path'
        snprintf(path, sizeof(path), "test/output/medium/medium%d.bmp", i + 1);
        write_bitmap(input_image, path);

        end = clock();

        if (read_correct)
        {
            cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
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
    int average_cells = total_cells / 10;
    double percentage_found = ((double)average_cells / TOTAL_CELLS) * 100;
    double average_time = (double)(total_time / pic_count);

    printf("---------------------------\n");
    printf("Average time %.3f s\n", average_time);
    printf("Average found cells: %d\n", average_cells);
    printf("Percentage of total cells: %.1f%%\n", percentage_found);
    printf("---------------------------\n");
}

void test_hard()
{
    int pic_count = 10;
    const char *sample_folder = "samples";
    const char *hard_folder = "hard";
    const char *input_filenames_hard[10] = {
        "1HARD.bmp",
        "2HARD.bmp",
        "3HARD.bmp",
        "4HARD.bmp",
        "5HARD.bmp",
        "6HARD.bmp",
        "7HARD.bmp",
        "8HARD.bmp",
        "9HARD.bmp",
        "10HARD.bmp"};

    clock_t start, end;
    double cpu_time_used;
    int total_cells = 0;
    double total_time = 0.0;

    printf("\n---------------------------\n");
    printf("         Hard Test       \n");
    printf("---------------------------\n");
    printf("%-10s %-10s %-10s\n", "Test", "Cells", "Time (s)");
    printf("---------------------------\n");

    for (int i = 0; i < 10; i++)
    {
        read_correct = 1;
        char input_filepath_hard[1024];
        snprintf(input_filepath_hard, sizeof(input_filepath_hard), "%s/%s/%s", sample_folder, hard_folder, input_filenames_hard[i]);

        char *argv[] = {
            "",
            ("%s", input_filepath_hard),
            ""};
        int argc = sizeof(argv) / sizeof(argv[0]);

        // For debugging purposes, you can print the filepath:
        // printf("File path: %s\n", input_filepath_easy);

        start = clock();
        run(argc, argv, 1);
        draw_x(input_image);
        char path[1024]; // Tildel en passende størrelse til 'path'
        snprintf(path, sizeof(path), "test/output/hard/hard%d.bmp", i + 1);
        write_bitmap(input_image, path);

        end = clock();

        if (read_correct)
        {
            cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
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
    int average_cells = total_cells / 10;
    double percentage_found = ((double)average_cells / TOTAL_CELLS) * 100;
    double average_time = (double)(total_time / pic_count);

    printf("---------------------------\n");
    printf("Average time %.3f s\n", average_time);
    printf("Average found cells: %d\n", average_cells);
    printf("Percentage of total cells: %.1f%%\n", percentage_found);
    printf("---------------------------\n");
}

void test_impossible()
{
    int pic_count = 5;
    const char *sample_folder = "samples";
    const char *impossible_folder = "impossible";
    const char *input_filenames_impossible[5] = {
        "1IMPOSSIBLE.bmp",
        "2IMPOSSIBLE.bmp",
        "3IMPOSSIBLE.bmp",
        "4IMPOSSIBLE.bmp",
        "5IMPOSSIBLE.bmp"};

    clock_t start, end;
    double cpu_time_used;
    int total_cells = 0;
    double total_time = 0.0;

    printf("\n---------------------------\n");
    printf("      Impossible Test      \n");
    printf("---------------------------\n");
    printf("%-10s %-10s %-10s\n", "Test", "Cells", "Time (s)");
    printf("---------------------------\n");

    for (int i = 0; i < 5; i++)
    {
        read_correct = 1;
        char input_filepath_impossible[1024];
        snprintf(input_filepath_impossible, sizeof(input_filepath_impossible), "%s/%s/%s", sample_folder, impossible_folder, input_filenames_impossible[i]);

        char *argv[] = {
            "",
            ("%s", input_filepath_impossible),
            ""};
        int argc = sizeof(argv) / sizeof(argv[0]);

        // For debugging purposes, you can print the filepath:
        // printf("File path: %s\n", input_filepath_easy);

        start = clock();
        run(argc, argv, 1);
        draw_x(input_image);
        char path[1024]; // Tildel en passende størrelse til 'path'
        snprintf(path, sizeof(path), "test/output/impossible/impossible%d.bmp", i + 1);
        write_bitmap(input_image, path);
        end = clock();

        if (read_correct)
        {
            cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
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
    int average_cells = total_cells / 5;
    double percentage_found = ((double)average_cells / TOTAL_CELLS) * 100;
    double average_time = (double)(total_time / pic_count);

    printf("---------------------------\n");
    printf("Average time %.3f s\n", average_time);
    printf("Average found cells: %d\n", average_cells);
    printf("Percentage of total cells: %.1f%%\n", percentage_found);
    printf("---------------------------\n");
}

int main()
{
    test_easy();
    test_medium();
    test_hard();
    test_impossible();

    return 0;
}
