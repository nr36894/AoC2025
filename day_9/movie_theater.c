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

bool _point_on_segment(point *p, point *p1, point *p2) {
    // Check if p is on the line segment from p1 to p2
    int min_x = MIN(p1->x, p2->x);
    int max_x = MAX(p1->x, p2->x);
    int min_y = MIN(p1->y, p2->y);
    int max_y = MAX(p1->y, p2->y);
    
    // Point must be within bounding box
    if (p->x < min_x || p->x > max_x || p->y < min_y || p->y > max_y) {
        return false;
    }
    
    // Check if point is collinear with the segment
    // For horizontal or vertical segments (which your problem has):
    if (p1->x == p2->x) {  // Vertical segment
        return p->x == p1->x;
    }
    if (p1->y == p2->y) {  // Horizontal segment
        return p->y == p1->y;
    }
    
    // For diagonal segments, check cross product = 0
    return (p->y - p1->y) * (p2->x - p1->x) == (p2->y - p1->y) * (p->x - p1->x);
}

bool _point_in_polygon(point *p, point *points, int num_points) {
    int x = p->x;
    int y = p->y;
    bool inside = false;
    
    int j = num_points - 1;  // Start with last vertex
    for (int i = 0; i < num_points; i++) {
        // Check if point is ON this edge
        if (_point_on_segment(p, &points[j], &points[i])) {
            if (TESTING) {
                printf("\t(%d, %d) Is on edge (%d, %d) -> (%d, %d)\n", 
                    x, y, points[j].x, points[j].y, points[i].x, points[i].y);
            }
            return true;
        }

        int xi = points[i].x;
        int yi = points[i].y;
        int xj = points[j].x;
        int yj = points[j].y;
        
        // Check if ray from point crosses this edge
        if ((yi > y) != (yj > y) && 
            (x < (xj - xi) * (y - yi) / (yj - yi) + xi)) {
            inside = !inside;
            
            if (TESTING) {
                printf("\t(%d, %d) Crosses line (%d, %d) -> (%d, %d)\n", x, y, points[j].x, points[j].y, points[i].x, points[i].y);
            }
        }
        

        j = i;
    }
    
    return inside;
}

bool _point_inside_rectangle(point *p1, point *p2, point *inside) {
    int min_x = MIN(p1->x, p2->x);
    int min_y = MIN(p1->y, p2->y);
    int max_x = MAX(p1->x, p2->x);
    int max_y = MAX(p1->y, p2->y);

    return (min_x < inside->x && inside->x < max_x && min_y < inside->y && inside->y < max_y);
}

bool _valid_square(point *p1, point *p2, point *points, int num_points) {
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

        for (int i = 0; i < 4; i++) {
            if ((corners[i].x == p1->x && corners[i].y == p1->y) ||
                (corners[i].x == p2->x && corners[i].y == p2->y)) {
                continue;
            }
            if (!_point_in_polygon(&corners[i], points, num_points)) {
                return false;
            }
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
            bool is_valid = _valid_square(&points[i], &points[j], points, num_points);

            if (area > biggest_area && is_valid) {
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

    if (TESTING) {
        printf("\n");
    }
    printf("Biggest Area: %llu\n", biggest_area);

    free(points);
}