#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>

#define TESTING false

int main() {
    FILE *fp;
    char line[3723];
    int line_length = 0;
    int num_lines = 0;
    int64_t homework_total = 0;

    if (TESTING) {
        fp = fopen("test.txt", "r");
    } else {
        fp = fopen("input.txt", "r");
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        if(line_length == 0) {
            line_length = strlen(line) - 1;
        }

        num_lines++;
    }

    char (*numbers)[num_lines] = malloc(sizeof(int[line_length][num_lines]));

    rewind(fp);

    int line_num = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        for (int i = 0; i < line_length; i++) {
            numbers[i][line_num] = line[i];
        }
        line_num++;
    }

    fclose(fp);

    int num_idx = 0;
    int num_list[10];
    memset(num_list, 0, sizeof(int) * 10);
    for (int col = line_length - 1; col >= 0; col--) {
        char *num_str = malloc(sizeof(char) + num_lines);
        int space_count = 0;

        for (int row = 0; row < num_lines - 1; row++) {
            num_str[row] = numbers[col][row];
            if (numbers[col][row] == ' ') {
                space_count++;
            }
        }
        if (numbers[col][num_lines - 1] == ' ') {
            space_count++;
        }

        num_str[num_lines - 1] = '\0';

        if (space_count == num_lines) {
            free(num_str);
            continue;
        }

        num_list[num_idx] = atoi(num_str);
        num_idx++;

        if (numbers[col][num_lines - 1] == '*') {
            int64_t problem_total = 0;
            for (int i = 0; i < num_idx; i++) {
                if (i == 0) {
                    problem_total = num_list[i];
                } else {
                    problem_total *= num_list[i];
                }
            }
            num_idx = 0;
            memset(num_list, 0, sizeof(int) * 10);
            homework_total += problem_total;
        } else if (numbers[col][num_lines - 1] == '+'){
            int64_t problem_total = 0;
            for (int i = 0; i < num_idx; i++) {
                if (i == 0) {
                    problem_total = num_list[i];
                } else {
                    problem_total += num_list[i];
                }
            }

            num_idx = 0;
            memset(num_list, 0, sizeof(int) * 10);
            homework_total += problem_total;
        }
        free(num_str);
    }

    printf("Grand total: %llu\n", homework_total);

    free(numbers);

    return 0;
}