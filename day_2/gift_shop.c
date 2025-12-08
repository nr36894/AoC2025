#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void splitString(const char *original, char **first_half, char **second_half) {
    int length = strlen(original);
    int mid_point = length/2;

    *first_half = (char *)malloc((mid_point + 1) * sizeof(char));
    if (*first_half == NULL) {
        perror("Failed to allocate memory for first half");
        exit(EXIT_FAILURE);
    }
    strncpy(*first_half, original, mid_point);
    (*first_half)[mid_point] = '\0';

    *second_half = (char *)malloc(((length - mid_point) + 1) * sizeof(char));
    if (*second_half == NULL) {
        perror("Failed to allocate memory for second half");
        free(*first_half);
        exit(EXIT_FAILURE);
    }

    strncpy(*second_half, original + mid_point, length - mid_point);
    (*second_half)[length - mid_point] = '\0';
}

int main() {
    FILE *fp;
    char line[512];
    char *tokens[1024];
    int num_tokens = 0;
    char *token;
    char str_num[50];
    u_int64_t first,last;
    u_int64_t total = 0;
    char *str_first, *str_end;
    u_int64_t invalid_ids[1024];
    int num_invalid_ids = 0;

    fp = fopen("input.txt", "r");

    if (fp == NULL) {
        perror("Could not open file. \n");
        return 1;
    }

    fgets(line, sizeof(line), fp);
    fclose(fp);

    printf("\n");

    token = strtok(line, ",");
    while (token != NULL) {
        printf("TOKEN: %s\n", token);
        tokens[num_tokens] = token;
        num_tokens++;
        token = strtok(NULL, ",");
    }

    printf("\n\n");

    for (int i = 0; i < num_tokens; i++) {
        num_invalid_ids = 0;

        printf("Token: %s\n", tokens[i]);

        char *endptr;

        first = strtoull(strtok(tokens[i], "-"), &endptr, 10);
        last = strtoull(strtok(NULL, "-"), &endptr, 10);

        for(u_int64_t j = first; j <= last; j++) {
            sprintf(str_num, "%llu", j);
            //printf("NUM: %s\n", str_num);
            if (strlen(str_num)%2 != 0) {
                continue;
            } else {
                splitString(str_num, &str_first, &str_end);

                if (atoi(str_first) == atoi(str_end)) {
                    invalid_ids[num_invalid_ids] = j;
                    num_invalid_ids++;
                }
            }
        }

        if (num_invalid_ids == 0) {
            printf("No invalid IDs.\n");
        } else {
            for (u_int64_t i = 0; i < num_invalid_ids; i++) {
                total += invalid_ids[i];
                printf("Invalid ID: %9llu, TOTAL: %10llu\n", invalid_ids[i], total);
            }
        }

        printf("\n\n");
    }

    printf("Total Invalid: %llu\n\n", total);

    free(str_first);
    free(str_end);

    return 0;
}