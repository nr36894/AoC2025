#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define TESTING true

int main() {
    FILE *fp;
    char line[220];
    int num_lines = 0;
    int line_length = 0;


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

    rewind(fp);
    
    fclose(fp);

    printf("Num Lines: %d\n", num_lines);
}