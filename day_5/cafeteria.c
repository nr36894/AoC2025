#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define TESTING false

typedef struct {
    uint64_t start;
    uint64_t end;
} Range;

void sort_range(Range *ranges, int num_ranges) {
    for (int i = 0; i < num_ranges - 1; i++) {
        for (int j = 0; j < num_ranges - i - 1; j++) {
            if (ranges[j].start > ranges[j + 1].start) {
                Range tmp = ranges[j];
                ranges[j] = ranges[j + 1];
                ranges[j + 1] = tmp;
            }
        }
    }
}

void merge_range(Range *ranges, int *num_ranges) {
    if (*num_ranges <= 1) return;

    int write_idx = 0;

    for (int i = 1; i < *num_ranges; i++) {
        if (ranges[i].start <= ranges[write_idx].end + 1) {
            if (ranges[i].end > ranges[write_idx].end) {
                ranges[write_idx].end = ranges[i].end;
            }
        } else {
            write_idx++;
            ranges[write_idx] = ranges[i];
        }
    }

    *num_ranges = write_idx + 1;
}

int main() {
    FILE *fp;
    char line[33];
    Range *ranges = NULL;
    Range *merged_ranges = NULL;
    int num_ranges = 0;
    int range_idx = 0;
    uint64_t fresh_ids = 0;

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
            break;
        }

        num_ranges++;
    }

    ranges = malloc(sizeof(Range) * num_ranges);

    rewind(fp);

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strcmp(line, "\n") == 0) {
            break;
        }

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
    }
    
    sort_range(ranges, num_ranges);
    merge_range(ranges, &num_ranges);

    printf("Num Ranges: %d\n", num_ranges);


    int num_valid_ranges = 0;
    if (true) {
        for (int i = 0; i < num_ranges; i++) {
            if (ranges[i].start == ranges[i].end && ranges[i].start == UINT64_MAX) {
                continue;
            }
            printf("Range: %llu-%llu\n", ranges[i].start, ranges[i].end);

            fresh_ids += ranges[i].end - ranges[i].start + 1;
            num_valid_ranges++;
        }
        printf("\n");
    }
    printf("Num Valid Ranges: %d\n", num_valid_ranges);

    printf("There are %llu fresh IDs.\n", fresh_ids);

    free(ranges);

    fclose(fp);

    return 0;
}