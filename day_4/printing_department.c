#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>

#define TESTING false

int main() {
    FILE *fp;
    char line[138];
    int line_length = 0;
    int num_lines = 0;
    int curr_line = 0;
    int num_accessable = -1;
    int total_removed = 0;
    

    if (TESTING) {
        fp = fopen("test.txt", "r");
    } else {
        fp = fopen("input.txt", "r");
    }

    if (!fp) {
        perror("Could not open file.\n");
        return 1;
    }

    if (TESTING) {
        printf("Initial state:\n");
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (line_length == 0) {
            line_length = strlen(line) - 1;
        }

        if (TESTING) {
            for (int i = 0; i < line_length; i++) {
                printf(" %c ", line[i]);
            }

            printf("\n");
        }

        num_lines++;
    }

    char (*warehouse_map)[num_lines] = malloc(sizeof(char[line_length][num_lines]));
    int (*warehouse)[num_lines] = malloc(sizeof(int[line_length][num_lines]));

    rewind(fp);

    while (fgets(line, sizeof(line), fp) != NULL) {
        for (int lineidx = 0; lineidx < line_length; lineidx++) {
            warehouse_map[lineidx][curr_line] = line[lineidx];
        }
        curr_line++;
    }

    fclose(fp);

    while (num_accessable != 0) {
        num_accessable = 0;

        for (int linenum = 0; linenum < num_lines; linenum++) {
            for (int lineidx = 0; lineidx < line_length; lineidx++) {
                warehouse[lineidx][linenum] = 0;
            }
        }

        for (curr_line = 0; curr_line < num_lines; curr_line++) {
            for (int lineidx = 0; lineidx < line_length; lineidx++) {
                if (warehouse_map[lineidx][curr_line] == '.') {
                    warehouse[lineidx][curr_line] = -9999;
                    continue;
                }

                if (lineidx - 1 >= 0) {
                    warehouse[lineidx - 1][curr_line]++;

                    if (curr_line - 1 >= 0) {
                        warehouse[lineidx - 1][curr_line - 1]++;
                    }

                    if (curr_line + 1 < num_lines) {
                        warehouse[lineidx - 1][curr_line + 1]++;
                    }
                }

                if (lineidx + 1 < line_length) {
                    warehouse[lineidx + 1][curr_line]++;

                    if (curr_line - 1 >= 0) {
                        warehouse[lineidx + 1][curr_line - 1]++;
                    }

                    if (curr_line + 1 < num_lines) {
                        warehouse[lineidx + 1][curr_line + 1]++;
                    }
                }

                if (curr_line - 1 >= 0) {
                    warehouse[lineidx][curr_line - 1]++;
                }

                if (curr_line + 1 < num_lines) {
                    warehouse[lineidx][curr_line + 1]++;
                }
            }
        }

        for (int linenum = 0; linenum < num_lines; linenum++) {
            for (int lineidx = 0; lineidx < line_length; lineidx++) {
                if (warehouse[lineidx][linenum] < 4 && warehouse[lineidx][linenum] >= 0) {
                    num_accessable++;
                }
            }
        }

        if (TESTING) {
            printf("\nRemove %d rolls of paper.\n", num_accessable);
        }
        total_removed += num_accessable;

        for (int linenum = 0; linenum < num_lines; linenum++) {
            for (int lineidx = 0; lineidx < line_length; lineidx++) {
                if (warehouse[lineidx][linenum] < 0) {
                    if (TESTING) {
                        printf("%2c ", '.');
                    }
                } else if (warehouse[lineidx][linenum] < 4) {
                    if (TESTING) {
                        printf("%2c ", 'x');
                    }
                    warehouse[lineidx][linenum] = -9999;
                    warehouse_map[lineidx][linenum] = '.';
                } else {
                    if (TESTING) {
                        printf("%2c ", '@');
                    }
                }
            }
            if (TESTING) {
                printf("\n");
            }
        }
    }

    if (TESTING) {
        printf("\n");
    }

    printf("%d rolls of paper can be removed.\n", total_removed);

    free(warehouse);
    free(warehouse_map);

    return 0;
}