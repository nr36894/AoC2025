#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define TESTING false

struct Range {
    uint64_t start;
    uint64_t end;
};

int main() {
    FILE *fp;
    char line[33];
    struct Range *ranges = NULL;
    uint64_t *ids = NULL;
    int num_ranges = 0;
    int range_idx = 0;
    int id_idx = 0;
    int num_ids = 0;
    int fresh_ids = 0;
    bool looking_at_ids = false;

    if (TESTING) {
        fp = fopen("test.txt", "r");
    } else {
        fp = fopen("input.txt", "r");
    }

    if (!fp) {
        perror("Could not open file.\n");
        return 1;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strcmp(line, "\n") == 0) {
            looking_at_ids = true;
            continue;
        }

        if (!looking_at_ids) {
            num_ranges++;
        } else {
            num_ids++;
        }
    }

    ranges = malloc(sizeof(struct Range) * num_ranges);
    ids = malloc(sizeof(int) * num_ids);

    rewind(fp);
    looking_at_ids = false;

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strcmp(line, "\n") == 0) {
            looking_at_ids = true;
            continue;
        }

        if (!looking_at_ids) {
            char *token;
            int token_num = 0;
            token = strtok(line, "-");

            while (token != NULL) {
                if (token_num == 0) {
                    char *endptr;
                    ranges[range_idx].start = strtoull(token, &endptr, 10);
                } else if (token_num == 1) {
                    char *endptr;
                    ranges[range_idx].end = strtoull(token, &endptr, 10);
                }
                token_num++;
                token = strtok(NULL, "-");
            }
            range_idx++;
        } else {
            char *endptr;
            ids[id_idx] =  strtoull(line, &endptr, 10);
            id_idx++;
        }
    }

    for (int curr_id_idx = 0; curr_id_idx < num_ids; curr_id_idx++) {
        uint64_t curr_id = ids[curr_id_idx];

        for (int curr_range_idx = 0; curr_range_idx < num_ranges; curr_range_idx++) {
            if (curr_id >= ranges[curr_range_idx].start && curr_id <= ranges[curr_range_idx].end) {
                fresh_ids++;
                break;
            }
        }
    }
    printf("Num Ranges: %d, Num IDs: %d\n", num_ranges, num_ids);

    if (TESTING) {
        for (int i = 0; i < num_ranges; i++) {
            printf("Range: %llu-%llu\n", ranges[i].start, ranges[i].end);
        }
        printf("\n");
        for (int i = 0; i < num_ids; i++) {
            printf("ID: %llu\n", ids[i]);
        }
    }

    printf("There are %d fresh IDs.\n", fresh_ids);

    free(ranges);
    free(ids);

    fclose(fp);

    return 0;
}