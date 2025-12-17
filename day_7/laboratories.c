#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>

#define TESTING false
#define GREEN "\033[0;32m"
#define NO_COL "\033[0m"

int tracePath(int row, int col, int line_length, int num_lines, void* map) {
    char (*tachyon_map)[num_lines] = map;
    char (*tmp_map)[num_lines] = NULL;

    if (TESTING) {
        char (*tmp_map)[num_lines] = malloc(sizeof(char[line_length][num_lines]));
        memcpy(tmp_map, tachyon_map, sizeof(char[line_length][num_lines]));
    } else {
        tmp_map = tachyon_map;
    }

    int total_paths = 0;

    for (;tachyon_map[col][row] != '^' && row < num_lines; row++) {
        if (TESTING) {
            if (tmp_map[col][row] == '.') {
                tmp_map[col][row] = '|';
            }
        }
    }

    if (row == num_lines) {
        total_paths++;

        if (TESTING) {
            for (int curr_row = 0; curr_row < num_lines; curr_row++) {
                for (int curr_col = 0; curr_col < line_length; curr_col++) {
                    if (curr_col == col && curr_row == row) {
                        printf("%s%c%s", GREEN, tmp_map[curr_col][curr_row], NO_COL);
                    } else {
                        printf("%c", tmp_map[curr_col][curr_row]);
                    }
                }
                printf("\n");
            }
            printf("\n");
        }

        if (TESTING) {
            free(tmp_map);
        }
        return total_paths;
    }

    total_paths += tracePath(row, col - 1, line_length, num_lines, tmp_map);
    total_paths += tracePath(row, col + 1, line_length, num_lines, tmp_map);

    if (TESTING) {
        free(tmp_map);
    }

    return total_paths;
}

int main() {
    FILE *fp;
    char line[143];
    int num_lines = 0;
    int line_length = 0;
    int split_count = 0;
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

    char (*tachyon_map)[num_lines] = malloc(sizeof(char[line_length][num_lines]));

    rewind(fp);

    int curr_line = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        for (int i = 0; i < strlen(line); i++) {
            if (line[i] != '\n') {
                tachyon_map[i][curr_line] = line[i];
            }
        }

        curr_line++;
    }

    for (int col = 0; col < line_length; col++) {
        if (tachyon_map[col][0] == 'S') {
            s_col = col;
            break;
        }
    }

    split_count = tracePath(0, s_col, line_length, num_lines, tachyon_map);

    printf("\n%d Splits.\n", split_count);

    fclose(fp);
    free(tachyon_map);

    return 0;
}