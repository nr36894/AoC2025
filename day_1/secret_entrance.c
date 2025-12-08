#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    char line[6];
    char turn_amt[5];
    int dialPos = 50;
    int direction = 0;
    int password = 0;
    int lineNum = 1;

    fp = fopen("input.txt", "r");

    if (fp == NULL) {
        printf("Error: Could not open file. \n");
        return 1;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (line[0] == 'L') {
            direction = -1;
        } else if (line[0] == 'R') {
            direction = 1;
        } else {
            direction = 0;
        }

        for (int i = 1; line[i] != '\n'; i++) {
            turn_amt[i-1] = line[i];
            turn_amt[i] = '\0';
        }

        for (int i = 0; i < atoi(turn_amt); i++) {
            dialPos = dialPos + (direction);
            dialPos = dialPos % 100;

            if (dialPos == 0) {
                password++;
            }
        }

        printf("Line %4d: %5d\n", lineNum, dialPos);
        lineNum++;
    }

    printf("\nPassword: %d\n", password);

    fclose(fp);

    return 0;
}