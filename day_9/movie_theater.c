#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define TESTING false
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct {
    int x;
    int y;
} point;

uint64_t _point_area(point *p1, point *p2) {
    return (uint64_t)(abs(p2->x - p1->x) + 1) * (uint64_t)(abs(p2->y - p1->y) + 1);
}

bool _segment_intersects_rectangle(point *rect1, point *rect2, point *poly1, point *poly2) {
    bool poly_vertical;
    bool rect_vertical;

    int rect_min_x = MIN(rect1->x, rect2->x);
    int rect_max_x = MAX(rect1->x, rect2->x);
    int rect_min_y = MIN(rect1->y, rect2->y);
    int rect_max_y = MAX(rect1->y, rect2->y);

    if (rect_min_x == rect_max_x) {
        rect_vertical = true;
    }

    int poly_min_x = MIN(poly1->x, poly2->x);
    int poly_max_x = MAX(poly1->x, poly2->x);
    int poly_min_y = MIN(poly1->y, poly2->y);
    int poly_max_y = MAX(poly1->y, poly2->y);

    if (poly_min_x == poly_max_x) {
        poly_vertical = true;
    }

    if (poly_vertical && rect_vertical) {
        return false;
    }

    // This is the only case I care about, so this is the only one I'm implementing
    if (rect_vertical) {
        // The version of x does not matter, since they must be the same
        return (poly_min_x < rect_min_x && rect_max_x < poly_max_x && rect_min_y < poly_min_y && rect_max_y > poly_max_y);
    }

    return false;
}

bool _point_inside_rectangle(point *p1, point *p2, point *inside) {
    int min_x = MIN(p1->x, p2->x);
    int min_y = MIN(p1->y, p2->y);
    int max_x = MAX(p1->x, p2->x);
    int max_y = MAX(p1->y, p2->y);

    return (min_x < inside->x && inside->x < max_x && min_y < inside->y && inside->y < max_y);
}

bool _valid_square(point *p1, point *p2, point *points, int num_points) {
    int min_x = MIN(p1->x, p2->x);
    int min_y = MIN(p1->y, p2->y);
    int max_x = MAX(p1->x, p2->x);
    int max_y = MAX(p1->y, p2->y);

    point corners[4] = {
        {min_x, min_y},
        {min_x, max_y},
        {max_x, min_y},
        {max_x, max_y}
    };

    for (int i = 0; i < num_points; i++) {
        if (&points[i] == p1 || &points[i] == p2) {
            continue;
        }

        if (_point_inside_rectangle(p1, p2, &points[i])) {
            if (TESTING) {
                printf("(%d, %d) within bounds.\n", points[i].x, points[i].y);
            }
            return false;
        }

        if (i + 1 == num_points) {
            if (_segment_intersects_rectangle(&corners[0], &corners[1], &points[i], &points[0])) {
                return false;
            }
        }
        if (_segment_intersects_rectangle(&corners[0], &corners[1], &points[i], &points[i+1])) {
            return false;
        }
    }

    return true;
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
    point *big_p1, *big_p2;
    for (int i = 0; i < num_points - 1; i++) {
        for (int j = i + 1; j < num_points; j++) {
            uint64_t area = _point_area(&points[i], &points[j]);

            if (area > biggest_area) {
                bool is_valid = _valid_square(&points[i], &points[j], points, num_points);
                if (is_valid) {
                    biggest_area = area;
                    big_p1 = &points[i];
                    big_p2 = &points[j];
                    if (TESTING) {
                        printf("*");
                    }
                }

                if (TESTING) {
                    printf("Is Valid: %s (%d, %d) -> (%d, %d) Area: %llu\n", is_valid ? "true" : "false", points[i].x, points[i].y, points[j].x, points[j].y, area);
                }
            }
        }
    }

    if (TESTING) {
        printf("\n");
    }
    printf("Biggest Area: %llu\n", biggest_area);
    printf("(%d, %d) -> (%d, %d)\n", big_p1->x, big_p1->y, big_p2->x, big_p2->y);

    free(points);
}