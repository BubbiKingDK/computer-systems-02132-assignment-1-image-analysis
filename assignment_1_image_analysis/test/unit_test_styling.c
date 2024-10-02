#include <stdio.h>
#include <string.h> 

// Farvekoder
#define RESET   "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m" 

typedef struct {
    int test_run;
    int test_passed;
    int test_failed;
    char failed_test[10][256];
    char failure_message[10][256];
} TestBlueprint;

void print_progress_bar(int current, int total, int passed, int failed) {
    int bar_width = 50;
    int passed_width = (bar_width * passed) / total; 
    int failed_width = (bar_width * failed) / total; 
    printf("\r[");
    for (int i = 0; i < passed_width; ++i) {
        printf(GREEN "=" RESET); 
    }

    for (int i = 0; i < failed_width; ++i) {
        printf(RED "=" RESET);
    }
    for (int i = passed_width + failed_width; i < bar_width; ++i) {
        printf(" ");
    }
    printf("] %d/%d", current, total);
}

void run_test(TestBlueprint *blueprint, const char *test_name, int (*test_func)(char *), char *failure_message, int total){
    blueprint->test_run++;

    int result = test_func(failure_message);
    if (result) {
        blueprint->test_passed++;
    } else {
        blueprint->test_failed++;

        // Gem navnet på den fejlede test
        snprintf(blueprint->failed_test[blueprint->test_failed - 1], 256, "%s", test_name);

        // Gem fejlmeddelelsen
        snprintf(blueprint->failure_message[blueprint->test_failed - 1], 256, "%s", failure_message);
    }

    // Opdater progress baren
    print_progress_bar(blueprint->test_run, total, blueprint->test_passed, blueprint->test_failed);
    Sleep(200);
}

void print_test_results(TestBlueprint blueprint) {
    double percentage_passed = ((double)blueprint.test_passed / blueprint.test_run) * 100;

    printf("\n\nTest Results:\n");
    printf("Tests run: %d\n", blueprint.test_run);

    printf( "Tests " GREEN "passed "RESET ": "  "%d\n" , blueprint.test_passed);
    if (blueprint.test_failed != 0) {
        printf( "Tests " RED "failed " RESET ": " "%d\n" , blueprint.test_failed);
    }
    
    printf("Success rate: %.2f%%\n", percentage_passed);

    // Hvis der er fejlede tests, udskriv deres navne og detaljer
    if (blueprint.test_failed > 0) {
        printf(RED "\nFailed tests:\n" RESET);
        for (int i = 0; i < blueprint.test_failed; i++) {
            printf("- %s: %s\n", blueprint.failed_test[i], blueprint.failure_message[i]);
        }
    }
    printf("\n");
} 
