#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int main() {
    FILE *fp;
    char line[138];
    int line_length = 0;
    int num_lines = 0;
    int curr_line = 0;
    int num_accessable = 0;
    

    fp = fopen("input.txt", "r");

    if (!fp) {
        perror("Could not open file.\n");
        return 1;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (line_length == 0) {
            line_length = strlen(line) - 1;
        }

        for (int i = 0; i < line_length; i++) {
            printf(" %c ", line[i]);
        }

        printf("\n");
        num_lines++;
    }

    int (*warehouse)[num_lines] = malloc(sizeof(int[line_length][num_lines]));

    for (int linenum = 0; linenum < num_lines; linenum++) {
        for (int lineidx = 0; lineidx < line_length; lineidx++) {
            warehouse[lineidx][linenum] = 0;
        }
    }

    rewind(fp);

    while (fgets(line, sizeof(line), fp) != NULL) {
        // if (curr_line == 0) {
        //     for (int lineidx = 1; lineidx < line_length; lineidx++) {
        //         warehouse[lineidx][0] = lineidx + 1;
        //     }
        // }

        // warehouse[0][curr_line] = curr_line + 1;
        // curr_line++;
        for (int lineidx = 0; lineidx < line_length; lineidx++) {
            if (line[lineidx] == '.') {
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
        curr_line++;
    }

    printf("\nline_length: %d, num_lines: %d\n\n", line_length, num_lines);

    for (int linenum = 0; linenum < num_lines; linenum++) {
        for (int lineidx = 0; lineidx < line_length; lineidx++) {
            if (warehouse[lineidx][linenum] < 0) {
                printf("%2c ", '.');
            } else if (warehouse[lineidx][linenum] < 4) {
                printf("%2c ", 'x');
                num_accessable++;
            } else {
                printf("%2c ", '@');
            }
        }
        printf("\n");
    }

    printf("\n");

    printf("There are %d rolls accessable.\n", num_accessable);

    fclose(fp);

    return 0;
}