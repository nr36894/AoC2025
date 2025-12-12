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
    int total = 0;

    fp = fopen("input.txt", "r");

    if (fp == NULL) {
        perror("Could not open file. \n");
        return 1;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        int first_val = 0;
        int first_idx = 0;
        int second_val = 0;
        int second_idx = 0;

        printf("%s", line);

        for (int i = 0; i < strlen(line) - 2; i++) {
            int val = line[i] - '0';

            if (val > first_val) {
                first_val = val;
                first_idx = i;
            }
        }

        for (int i = first_idx + 1; i < strlen(line) - 1; i++) {
            int val = line[i] - '0';

            if (val > second_val) {
                second_val = val;
                second_idx = i;
            }
        }

        total += first_val * 10 + second_val;
        printf("Max: %d%d\n", first_val, second_val);
    }

    printf("%d\n", total);

    return 1;
}