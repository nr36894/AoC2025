#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    FILE *fp;
    char line[6];
    char turn_amt[5];
    int turn_val;
    int dialPos = 50;
    int direction = 0;
    int password = 0;
    int lineNum = 1;
    int pw_change;
    bool triggered_pass = false;

    fp = fopen("input.txt", "r");

    if (fp == NULL) {
        printf("Error: Could not open file. \n");
        return 1;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        triggered_pass = false;
        pw_change = 0;

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

        turn_val = atoi(turn_amt) * direction;

        if (direction < 0) {
            if (abs(turn_val) >= dialPos && dialPos != 0) {
                turn_val = turn_val + dialPos;
                dialPos = 0;
                password++;
                pw_change++;
                triggered_pass = true;
            }
        } else if (direction > 0) {
            if (turn_val >= (100 - dialPos) && dialPos != 0) {
                turn_val = dialPos - (100 - turn_val);
                dialPos = 0;
                password++;
                pw_change++;
                triggered_pass = true;
            }
        }

        int change = abs(turn_val / 100);
        if (change > 0) {
            password += change;
            pw_change += change;
            triggered_pass = true;
        }
        dialPos += turn_val;
        dialPos = dialPos % 100;

        if (dialPos < 0) {
            dialPos = 100 + dialPos;
        }

        printf("Line %4d: %3d %5s %d\n", lineNum, dialPos, triggered_pass ? "true" : "false", pw_change);
        lineNum++;
    }

    printf("\nPassword: %d\n", password);

    fclose(fp);

    return 0;
}