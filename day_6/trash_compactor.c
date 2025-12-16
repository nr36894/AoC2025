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
            char *token;
            token = strtok(line, " ");

            while (token != NULL) {
                line_length++;
                token = strtok(NULL, " ");
            }
        }

        num_lines++;
    }

    int (*numbers)[num_lines] = malloc(sizeof(int[line_length][num_lines]));
    char **symbols = malloc(sizeof(char *) * line_length);

    rewind(fp);

    int line_num = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        char *token;
        int line_idx = 0;
        token = strtok(line, " ");

        while (token != NULL) {
            if (line_num == num_lines - 1) {
                symbols[line_idx] = malloc(sizeof(char) * strlen(token));
                strcpy(symbols[line_idx], token);
            } else {
                numbers[line_idx][line_num] = atoi(token);
            }

            token = strtok(NULL, " ");
            line_idx++;
        }
        line_num++;
    }

    fclose(fp);

    for (int column = 0; column < line_length; column++) {
        int64_t problem_total = 0;
        for (int row = 0; row < num_lines - 1; row++) {
            printf("%d %s ", numbers[column][row], symbols[column]);

            if (row == 0) {
                problem_total = numbers[column][row];
            } else if (strcmp(symbols[column], "*") == 0) {
                problem_total *= numbers[column][row];
            } else if (strcmp(symbols[column], "+") == 0) {
                problem_total += numbers[column][row];
            } 
        }
        printf("= %llu\n", problem_total);
        homework_total += problem_total;
    }

    printf("Grand total: %llu\n", homework_total);

    for (int i = 0; i < line_length; i++) {
        free(symbols[i]);
    }

    free(numbers);
    free(symbols);

    return 0;
}