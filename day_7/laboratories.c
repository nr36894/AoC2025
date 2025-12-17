#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>

#define TESTING false

int main() {
    FILE *fp;
    char line[143];
    int num_lines = 0;
    int line_length = 0;
    uint64_t split_count = 0;
    int s_col = 0;


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

    uint64_t *num_beams = malloc(sizeof(int[line_length]));

    rewind(fp);

    while (fgets(line, sizeof(line), fp) != NULL) {
        for (int i = 0; i < line_length; i++) {
            if (line[i] == 'S') {
                num_beams[i] = 1;
            } else if (line[i] == '^') {
                num_beams[i-1] += num_beams[i];
                num_beams[i+1] += num_beams[i];
                num_beams[i] = 0;
            }
        }

        if (TESTING) {
            for (int i = 0; i < line_length; i++) {
                printf("%2llu ", num_beams[i]);
            }
            printf("\n");
        }
    }

    for (int i = 0; i < line_length; i++){
        split_count += num_beams[i];
    }

    printf("\n%llu Splits.\n", split_count);

    fclose(fp);
    free(num_beams);

    return 0;
}