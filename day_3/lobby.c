#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int* pairNumbers(int total) {
    static int pair[2];

    return pair;
}

int main() {
    FILE *fp;
    char line[126];
    uint64_t total = 0;
    uint64_t ull_highest_num = 0;

    fp = fopen("input.txt", "r");

    if (fp == NULL) {
        perror("Could not open file. \n");
        return 1;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        int *int_line = (int*)malloc((strlen(line) - 1) * sizeof(int));
        int line_length = strlen(line) - 1;

        for (int i = 0; i < line_length; i++) {
            int_line[i] = line[i] - '0';
        }

        for (int i = 0; i < line_length; i++) {
            if (int_line[i] > 0) {
                printf("%d", int_line[i]);
            }
        }
        printf("\n");

        char highest_num[13];
        memset(highest_num, '-', sizeof(highest_num));
        highest_num[12] = '\0';
        int highest_num_idx = 0;
        int remaining_picks = 12;
        int start = 0;
        int end = line_length - remaining_picks;

        // Window Loop
        while (end < line_length - 1) {
            int max_int = 0;
            int max_idx = -1;
            end = line_length - remaining_picks;
            for (int i = start; i <= end; i++) {
                if (int_line[i] > max_int) {
                    max_int = int_line[i];
                    max_idx = i;
                    start = i + 1;
                }
            }

            highest_num[highest_num_idx] = max_int + '0';
            highest_num_idx++;
            remaining_picks--;
            printf("Highest Num: %s!\n", highest_num);
        }

        printf("\n");
        printf("%s\n", highest_num);
        
        char *endptr;
        ull_highest_num = strtoull(highest_num, &endptr, 10);
        total += ull_highest_num;

        free(int_line);
    }

    printf("\nTotal: %llu\n", total);

    return 1;
}