#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int countSubstring(const char *str, const char *sub) {
    int count = 0;
    const char *tmp = str;
    while ((tmp = strstr(tmp, sub)) != NULL) {
        count ++;
        tmp += strlen(sub);
    }
    return count;
}

int main() {
    FILE *fp;
    char line[512];
    char substring[256];
    int sub_idx = 0;
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
        tokens[num_tokens] = token;
        num_tokens++;
        token = strtok(NULL, ",");
    }

    for (int i = 0; i < num_tokens; i++) {
        num_invalid_ids = 0;

        printf("Token: %s\n", tokens[i]);

        char *endptr;

        first = strtoull(strtok(tokens[i], "-"), &endptr, 10);
        last = strtoull(strtok(NULL, "-"), &endptr, 10);

        for(u_int64_t j = first; j <= last; j++) {
            sprintf(str_num, "%llu", j);
            //printf("NUM: %s\n", str_num);

            int length = strlen(str_num);
            int mid_point = (length) / 2;

            for (int index = 0; index < mid_point; index++) {
                int end = index;
                int start = 0;
                sub_idx = 0;

                for (; start <= end; start++) {
                    substring[sub_idx] = str_num[start];
                    sub_idx++;
                }

                substring[sub_idx] = '\0';
                //printf("SUBSTRING: %s\n", substring);

                int times_repeated = countSubstring(str_num, substring);
                int required_repeat = strlen(str_num) / strlen(substring);
                int remaining_length = strlen(str_num) % strlen(substring);

                if (remaining_length > 0) {
                    //printf("%llu Cannot be invalid\n\n", j);
                    continue;
                }

                //printf("Times Repated: %d, Required Repeat: %d\n", times_repeated, required_repeat);

                if (times_repeated == required_repeat) {
                    printf("INVALID ID FOUND: %llu\n", j);
                    invalid_ids[num_invalid_ids] = j;
                    num_invalid_ids++;
                    break;
                }
                //printf("\n");

            }
        }

        if (num_invalid_ids == 0) {
            printf("No invalid IDs.\n");
        } else {
            for (u_int64_t i = 0; i < num_invalid_ids; i++) {
                total += invalid_ids[i];
                //printf("Invalid ID: %9llu, TOTAL: %10llu\n", invalid_ids[i], total);
            }
        }

        printf("\n");
    }

    printf("Total Invalid: %llu\n\n", total);

    free(str_first);
    free(str_end);

    return 0;
}