#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#define TESTING false

typedef struct {
    int num_lights;
    int num_buttons;
    int num_joltages;
    bool lights [10];
    bool buttons [12][10];
    int voltages [10];
} machine;

void _replace_char(char *s, char c1, char c2) {
    int l = strlen(s);

    for (int i = 0; i < l; i++) {
        if (s[i] == c1) {
            s[i] = c2;
        }
    }
}

void _parse_line(char *s, machine *curr_machine) {
    bool in_lights = false;
    bool in_buttons = false;
    bool in_voltages = false;
    int index = 0;
    int button_index = 0;
    int voltage_index = 0;
    char voltage_str[4] = {'\0'};
    int voltage_str_idx = 0;
    int strLen = strlen(s);

    for (int i = 0; i < strLen; i++) {
        switch (s[i]) {
            case '[':
                in_lights = true;
                index = 0;
                break;
            case ']':
                in_lights = false;
                break;
            case '(':
                in_buttons = true;
                index = 0;
                break;
            case ')':
                in_buttons = false;
                button_index++;
                curr_machine->num_buttons = button_index;
                break;
            case '{':
                in_voltages = true;
                index = 0;
                break;
            case '}':
                curr_machine->voltages[voltage_index] = atoi(voltage_str);
                for (int c = 0; c < 4; c++) {
                    voltage_str[c] = '\0';
                }
                voltage_str_idx = 0;
                voltage_index++;
                curr_machine->num_joltages = voltage_index;
                in_voltages = false;
                break;
            default:
                if (in_lights && s[i] == '.') {
                    curr_machine->lights[index] = false;
                    index++;
                    curr_machine->num_lights = index;
                } else if (in_lights && s[i] == '#') {
                    curr_machine->lights[index] = true;
                    index++;
                    curr_machine->num_lights = index;
                } else if (in_voltages && s[i] == ',') {
                    curr_machine->voltages[voltage_index] = atoi(voltage_str);
                    for (int c = 0; c < 4; c++) {
                        voltage_str[c] = '\0';
                    }
                    voltage_str_idx = 0;
                    voltage_index++;
                    curr_machine->num_joltages = voltage_index;
                } else if (in_voltages && s[i] != ',') {
                    voltage_str[voltage_str_idx] = s[i];
                    voltage_str_idx++;
                } else if (in_buttons) {
                    switch (s[i]) {
                        case ',':
                        case ' ':
                            break;
                        default:
                            curr_machine->buttons[button_index][s[i] - '0'] = 1;
                            index++;
                            break;
                    }
                }
        }
    }
}

int _solve_matrix(machine *curr_machine, void *v_matrix) {
    int (*matrix)[curr_machine->num_buttons + 1] = v_matrix;
    bool is_free[curr_machine->num_buttons];
    int free_idx[curr_machine->num_buttons];
    int free_max[curr_machine->num_buttons];
    int rhs[curr_machine->num_joltages];
    int num_free = curr_machine->num_buttons;
    int min_presses = INT_MAX;

    memset(is_free, true, sizeof(is_free));
    memset(rhs, false, sizeof(rhs));
    memset(free_idx, 0, sizeof(free_idx));

    for (int i = 0; i < curr_machine->num_joltages; i++) {
        rhs[i] = matrix[i][curr_machine->num_buttons];
    }

    for (int i = 0; i < curr_machine->num_buttons; i++) {
        free_max[i] = -1;
    }

    for (int row = 0; row < curr_machine->num_joltages; row++) {
        for (int col = 0; col < curr_machine->num_buttons; col++) {
            
            if (matrix[row][col] != 0) {
                is_free[col] = false;
                num_free--;
                break;
            }
        }
    }

    int idx = 0;
    for (int col = 0; col < curr_machine->num_buttons; col++) {
        if (is_free[col]) {
            free_idx[idx++] = col;
        }
    }


    // Verify state of math variables
    if (TESTING || !TESTING) {
        printf("\nRHS: [ ");
        for (int i = 0; i < curr_machine->num_joltages; i++) {
            printf("%d ", rhs[i]);
        }
        printf("]\n");

        printf("Total Free Vars: %d\n", num_free);

        printf("Free Vars: [ ");
        for (int i = 0; i < curr_machine->num_buttons; i++) {
            printf("%s ", (is_free[i]) ? "t" : "f");
        }
        printf("]\n");
        printf("Free Cols: [ ");
        for (int i = 0; i < curr_machine->num_buttons; i++) {
            printf("%d ", free_idx[i]);
        }
        printf("]\n\n");
    }

    for (int row = 0; row < curr_machine->num_joltages; row++) {
        for (int col = 0; col < curr_machine->num_buttons; col++) {
            if (matrix[row][col] != 0 && is_free[col]) {
                if (matrix[row][col] > 0 && rhs[row] > free_max[col]) {
                    free_max[col] = rhs[row];
                }
            }
        }
    }

    if (TESTING || !TESTING) {
        printf("Free Max: [ ");
        for (int i = 0; i < curr_machine->num_buttons; i++) {
            printf("%d ", free_max[i]);
        }
        printf("]\n\n");
        for (int i = 0; i < curr_machine->num_buttons; i++) {
            if (!is_free[i]) {
                continue;
            }
            printf("  ");
            printf("b%-2d", i);
            if (free_max[i] != INT_MAX) {
                printf(" ≤ %-3d", free_max[i]);
            }
            printf("\n");

        }
        printf("\n");
    }

    // for (int i = 0)

    return min_presses;
}

int _create_echelon_matrix(machine *curr_machine) {
    int (*matrix)[curr_machine->num_buttons + 1] = malloc(curr_machine->num_joltages * sizeof(*matrix));
    memset(matrix, false, sizeof(*matrix));

    for (int i = 0; i < curr_machine->num_joltages; i++) {
        for (int j = 0; j < curr_machine->num_buttons + 1; j++) {
            matrix[i][j] = curr_machine->buttons[j][i];
        }
            matrix[i][curr_machine->num_buttons] = curr_machine->voltages[i];
    }

    if (TESTING || !TESTING) {
        printf("Starting Matrix:\n");
        for (int i = 0; i < curr_machine->num_joltages; i++) {
            printf("| ");
            for (int j = 0; j < curr_machine->num_buttons + 1; j++) {
                if (j == curr_machine->num_buttons) {
                    printf("| %3d ", matrix[i][j]);
                } else {
                    printf("%2d ", matrix[i][j]);
                }
            }
            printf("|\n");
        }
    }


    // Reduce the matrix
    for (int col = 0; col < curr_machine->num_buttons + 1; col++) {
        int pivot_row = -1;
        int curr_row = col;
        for (int r = curr_row; r < curr_machine->num_joltages; r++) {
            if (matrix[r][col] == 1) {
                pivot_row = r;
                break;
            } else if (matrix[r][col] == -1) {
                pivot_row = r;
                for (int i = 0; i < curr_machine->num_buttons + 1; i++) {
                    matrix[r][i] *= -1;
                }
                break;
            }
        }

        if (pivot_row == -1) continue;

        if (pivot_row != curr_row) {
            for (int c = 0; c < curr_machine->num_buttons + 1; c++) {
                matrix[curr_row][c] = matrix[curr_row][c] ^ matrix[pivot_row][c];
                matrix[pivot_row][c] = matrix[curr_row][c] ^ matrix[pivot_row][c];
                matrix[curr_row][c] = matrix[curr_row][c] ^ matrix[pivot_row][c];
            }
            pivot_row = curr_row;
        }

        for (int row = 0; row < curr_machine->num_joltages; row++) {
            if (row != pivot_row && matrix[row][col] != 0) {
                int row_coef = matrix[row][col];
                int pivot_coef = matrix[pivot_row][col];

                for (int c = 0; c < curr_machine->num_buttons + 1; c++) {
                    matrix[row][c] = matrix[row][c] * pivot_coef - matrix[pivot_row][c] * row_coef;
                }
            }
        }
        if (TESTING) {
        printf("\nCurrent Matrix:\n");
            for (int i = 0; i < curr_machine->num_joltages; i++) {
                printf("| ");
                for (int j = 0; j < curr_machine->num_buttons + 1; j++) {
                    if (j == curr_machine->num_buttons) {
                        printf("| %3d ", matrix[i][j]);
                    } else {
                        printf("%2d ", matrix[i][j]);
                    }
                }
                printf("|\n");
            }
        }
    }

    if (TESTING || !TESTING) {
        printf("\nEnding Matrix:\n");
        for (int i = 0; i < curr_machine->num_joltages; i++) {
            printf("| ");
            for (int j = 0; j < curr_machine->num_buttons + 1; j++) {
                if (j == curr_machine->num_buttons) {
                    printf("| %3d ", matrix[i][j]);
                } else {
                    printf("%2d ", matrix[i][j]);
                }
            }
            printf("|\n");
        }
    }

    int answer = _solve_matrix(curr_machine, matrix);

    free(matrix);

    return answer;
}

int main() {
    FILE *fp;
    char line[220];
    int num_lines = 0;
    int line_length = 0;
    int total_answer = 0;


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

    machine *machines = malloc(sizeof(machine) * num_lines);
    memset(machines, 0, sizeof(machine) * num_lines);

    int line_num = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        _parse_line(line, &machines[line_num]);

        line_num++;
    }
    
    fclose(fp);

    for (int i = 0; i < num_lines; i++) {
        printf("Light Pattern:    ");
        for (int j = 0; j < machines[i].num_lights; j++) {
            printf("%2d ", machines[i].lights[j]);
        }
        printf("\n");
        printf("Joltages:         ");
        for (int j = 0; j < machines[i].num_joltages; j++) {
            printf("%2d ", machines[i].voltages[j]);
        }
        printf("\n");
        for (int j = 0; j < machines[i].num_buttons; j++) {
            printf("       Button %2d: ", j);
            for (int k = 0; k < machines[i].num_lights; k++) {
                printf("%2d ", machines[i].buttons[j][k]);
            }
            printf("\n");
        }
        printf("\n");
        printf("\n");

        int answer = _create_echelon_matrix(&machines[i]);
        total_answer += answer;
        printf("Answer: %d\n", answer);
        printf("\n");
    }

    printf("Num Lines: %d\n", num_lines);
    printf("Final Answer: %d\n", total_answer);
}