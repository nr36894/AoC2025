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
    int split_count = 0;


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

    for (int row = 0; row < num_lines; row++) {
        for (int col = 0; col < line_length; col++) {
            if (tachyon_map[col][row] == 'S' && tachyon_map[col][row + 1] == '.') {
                tachyon_map[col][row + 1] = '|';
            } else if (tachyon_map[col][row] == '^' && tachyon_map[col][row - 1] == '|') {
                split_count++;
                if (col + 1 < line_length) {
                    if (tachyon_map[col + 1][row - 1] == '|') {
                        tachyon_map[col - 1][row] = '|';
                    } else if (tachyon_map[col + 1][row] == '.') {
                        tachyon_map[col + 1][row] = '|';
                    }
                }
                if (col - 1 >= 0) {
                    if (tachyon_map[col - 1][row - 1] == '|') {
                        tachyon_map[col - 1][row] = '|';
                    } else if (tachyon_map[col - 1][row] == '.') {
                        tachyon_map[col - 1][row] = '|';
                    }
                }
            } else if (tachyon_map[col][row - 1] == '|') {
                tachyon_map[col][row] = '|';
            }
        }

        if (TESTING) {
            for (int row = 0; row < num_lines; row++) {
                for (int col = 0; col < line_length; col++) {
                    printf("%c", tachyon_map[col][row]);
                }
                printf("\n");
            }

            printf("%d Splits.\n\n", split_count);
        }
    }

    for (int row = 0; row < num_lines; row++) {
        for (int col = 0; col < line_length; col++) {
            printf("%c", tachyon_map[col][row]);
        }
        printf("\n");
    }

    printf("\n%d Splits.\n", split_count);

    fclose(fp);
    free(tachyon_map);

    return 0;
}