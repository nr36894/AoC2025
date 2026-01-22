#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#define TESTING true

typedef struct {
    int num_lights;
    int num_buttons;
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
    bool in_lights;
    bool in_buttons;
    bool in_voltages;
    int index = 0;
    int button_index = 0;
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
    int button_state [curr_machine->num_buttons];
    bool (*matrix)[curr_machine->num_buttons + 1] = v_matrix;
    bool is_free[curr_machine->num_buttons];
    int free_idx[curr_machine->num_buttons];
    bool rhs[curr_machine->num_lights];
    int num_free = 0;
    int min_presses = INT_MAX;

    memset(is_free, false, sizeof(is_free));
    memset(rhs, false, sizeof(rhs));
    memset(free_idx, 0, sizeof(free_idx));

    for (int i = 0; i < curr_machine->num_lights; i++) {
        rhs[i] = matrix[i][curr_machine->num_buttons];
    }

    for (int col = 0; col < curr_machine->num_buttons; col++) {
        bool has_pivot = false;
        for (int row = 0; row < curr_machine->num_lights; row++) {
            if (matrix[row][col] == 1 && row == col) {
                has_pivot = true;
                break;
            }
        }

        if (!has_pivot) {
            is_free[col] = true;
            free_idx[num_free] = col;
            num_free++;
        }
    }

    // Verify state of math variables
    if (TESTING) {
        printf("RHS: [ ");
        for (int i = 0; i < curr_machine->num_lights; i++) {
            printf("%d ", rhs[i]);
        }
        printf("]\n");

        printf("Total Free Vars: %d\n", num_free);

        printf("Free Vars: [ ");
        for (int i = 0; i < curr_machine->num_buttons; i++) {
            printf("%s ", (is_free[i]) ? "t" : "f");
        }
        printf("]\n");
    }

    int max_free = pow(2, num_free);

    
    for (int i = 0; i < max_free; i++) {
        bool solution[curr_machine->num_buttons];
        memset(solution, false, sizeof(solution));

        for (int idx = 0; idx < num_free; idx++) {
            solution[free_idx[idx]] = (i >> idx) & 1;
        }

        for (int row = 0; row < curr_machine->num_lights; row++) {
            if (matrix[row][row] == 1) {
                bool val = matrix[row][curr_machine->num_buttons];
                for (int col = 0; col < curr_machine->num_buttons; col++) {
                    if (col != row && matrix[row][col] == 1) {
                        val ^= solution[col];
                    }
                }
                solution[row] = val;
            }
        }

        bool valid_solution = true;
        for (int light = 0; light < curr_machine->num_lights; light++){
            bool light_state = false;
            for (int button = 0; button < curr_machine->num_buttons; button++) {
                if (matrix[light][button] == 1) {
                    light_state ^= solution[button];
                }
            }

            if (light_state != rhs[light]) {
                if (TESTING || !TESTING) {
                    printf("INVALID ");
                }
                valid_solution = false;
                break;
            }
        }

        if (TESTING || !TESTING) {
            printf("Solution: ");
            for (int button = 0; button < curr_machine->num_buttons; button++) {
                printf("%d ", solution[button]);
            }
            printf("\n");
        }

        if (valid_solution) {
            int num_presses = 0;
            for (int j = 0; j < curr_machine->num_buttons; j++) {
                if (solution[j]) {
                    num_presses++;
                }
            }

            if (num_presses < min_presses && num_presses > 0) {
                min_presses = num_presses;
            }
        }
    }

    return min_presses;
}

int _time_to_gaussian(machine *curr_machine) {
    bool (*matrix)[curr_machine->num_buttons + 1] = malloc(curr_machine->num_lights * sizeof(*matrix));
    memset(matrix, false, sizeof(*matrix));

    for (int i = 0; i < curr_machine->num_lights; i++) {
        for (int j = 0; j < curr_machine->num_buttons + 1; j++) {
            matrix[i][j] = curr_machine->buttons[j][i];
        }
            matrix[i][curr_machine->num_buttons] = curr_machine->lights[i];
    }

    if (TESTING) {
        printf("Starting Matrix:\n");
        for (int i = 0; i < curr_machine->num_lights; i++) {
            printf("| ");
            for (int j = 0; j < curr_machine->num_buttons + 1; j++) {
                if (j == curr_machine->num_buttons) {
                    printf("| %d ", matrix[i][j]);
                } else {
                    printf("%d ", matrix[i][j]);
                }
            }
            printf("|\n");
        }
    }

    for (int col = 0; col < curr_machine->num_buttons + 1; col++) {
        int pivot_row = -1;
        int curr_row = col;
        for (int r = curr_row; r < curr_machine->num_lights; r++) {
            if (matrix[r][col] == 1) {
                pivot_row = r;
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

        for (int row = 0; row < curr_machine->num_lights; row++) {
            if (row != pivot_row && matrix[row][col] == 1) {
                for (int c = 0; c < curr_machine->num_buttons + 1; c++) {
                    matrix[row][c] = matrix[row][c] ^ matrix[pivot_row][c];
                }
            }
        }
        if (TESTING) {
        printf("\nCurrent Matrix:\n");
            for (int i = 0; i < curr_machine->num_lights; i++) {
                printf("| ");
                for (int j = 0; j < curr_machine->num_buttons + 1; j++) {
                    if (j == curr_machine->num_buttons) {
                        printf("| %d ", matrix[i][j]);
                    } else {
                        printf("%d ", matrix[i][j]);
                    }
                }
                printf("|\n");
            }
        }
    }

    if (TESTING || !TESTING) {
        printf("\nEnding Matrix:\n");
        for (int i = 0; i < curr_machine->num_lights; i++) {
            printf("| ");
            for (int j = 0; j < curr_machine->num_buttons + 1; j++) {
                if (j == curr_machine->num_buttons) {
                    printf("| %d ", matrix[i][j]);
                } else {
                    printf("%d ", matrix[i][j]);
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
            printf("%d ", machines[i].lights[j]);
        }
        printf("\n");
        for (int j = 0; j < machines[i].num_buttons; j++) {
            printf("       Button %2d: ", j);
            for (int k = 0; k < machines[i].num_lights; k++) {
                printf("%d ", machines[i].buttons[j][k]);
            }
            printf("\n");
        }
        printf("\n");

        int answer = _time_to_gaussian(&machines[i]);
        total_answer += answer;
        printf("Answer: %d", answer);
        printf("\n");
    }

    printf("Num Lines: %d\n", num_lines);
    printf("Final Answer: %d\n", total_answer);
}