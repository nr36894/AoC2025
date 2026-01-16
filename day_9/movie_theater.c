#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define TESTING false

typedef struct {
    int x;
    int y;
} point;

uint64_t _point_area(point *p1, point *p2) {
    return (uint64_t)(abs(p2->x - p1->x) + 1) * (uint64_t)(abs(p2->y - p1->y) + 1);
}

int main() {
    FILE *fp;
    char line[13];
    int num_points = 0;
    int line_length = 0;


    if (TESTING) {
        fp = fopen("test.txt", "r");
    } else {
        fp = fopen("input.txt", "r");
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        num_points++;
    }

    rewind(fp);

    point *points = malloc(sizeof(point) * num_points);
    
    int point_idx = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        char *token;

        token = strtok(line, ",");
        points[point_idx].x = atoi(token);

        token = strtok(NULL, ",");
        points[point_idx].y = atoi(token);

        point_idx++;
    }

    if (TESTING) {
        printf("\nPoints:\n");
        for (int i = 0; i < num_points; i++) {
            printf("\tx: %-5d y: %-5d\n", points[i].x, points[i].y);
        }
        printf("\n");
    }

    uint64_t biggest_area = 0;
    point *p1, *p2;
    for (int i = 0; i < num_points - 1; i++) {
        for (int j = i + 1; j < num_points; j++) {
            uint64_t area = _point_area(&points[i], &points[j]);

            if (area > biggest_area) {
                biggest_area = area;
                p1 = &points[i];
                p2 = &points[j];
            }

            if (TESTING) {
                printf("(%d, %d) -> (%d, %d) Area: %llu\n", points[i].x, points[i].y, points[j].x, points[j].y, area);
            }
        }
    }

    if (TESTING) {
        printf("\n");
    }

    printf("Biggest Area: %llu\n", biggest_area);

    free(points);
}