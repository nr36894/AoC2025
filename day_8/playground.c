#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <float.h>

#define TESTING true

typedef struct {
    int x;
    int y;
    int z;
    struct point_3d *next;
    struct point_3d *prev;
} point_3d;

double _point3d_distance(point_3d *p1, point_3d *p2) {
    return sqrt(pow((p1->x - p2->x), 2) + pow((p1->y - p2->y), 2) + pow((p1->z - p2->z), 2));
}

void _print_point(point_3d *p) {
    printf("(%d, %d, %d)", p->x, p->y, p->z);
}

void _print_distance_table(void *table, int num_lines) {
    double (*distance_table)[num_lines] = table;

    for (int primary = 0; primary < num_lines; primary++) {
        if (primary == 0) {
            printf("|     |");

            for (int col_num = 0; col_num < num_lines; col_num++) {
                printf(" %7d|", col_num);
            }
            printf("\n+-----+");
            for (int col_num = 0; col_num < num_lines; col_num++) {
                printf("--------+");
            }
            printf("\n");
        }

        printf("| %4d", primary);
        for (int secondary = 0; secondary < num_lines; secondary++) {
            if (secondary == 0) {
                printf("|");
            }

            printf(" %7.2f|", distance_table[primary][secondary]);
        }
        printf("\n");
    }
}

int main() {
    FILE *fp;
    char line[19];
    int num_lines = 0;
    int line_length = 0;
    uint64_t split_count = 0;
    int s_col = 0;


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

    point_3d *points = malloc(sizeof(point_3d) * num_lines);
    int *closest_idx = malloc(sizeof(int) * num_lines);
    double (*distance_table)[num_lines] = malloc(sizeof(double[num_lines][num_lines]));

    for (int i = 0; i < num_lines; i++) {
        closest_idx[i] = INT_MIN;
    }

    int point_idx = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        char *token;

        token = strtok(line, ",");
        points[point_idx].x = atoi(token);

        token = strtok(NULL, ",");
        points[point_idx].y = atoi(token);

        token = strtok(NULL, ",");
        points[point_idx].z = atoi(token);

        point_idx++;
    }
    
    fclose(fp);

    for (int primary = 0; primary < num_lines; primary++) {
        if (primary + 1 == num_lines) {
            break;
        }
        for (int secondary = primary + 1; secondary < num_lines; secondary++) {
            if (primary == secondary) {
                distance_table[primary][secondary] = 0;
                continue;
            }

            distance_table[primary][secondary] = _point3d_distance(&points[primary], &points[secondary]);
        }
    }

    printf("\n");

    _print_distance_table(distance_table, num_lines);

    printf("\n");
    
    for (int i = 0; i < 10; i++) {
        double shortest_distance = DBL_MAX;
        int shortest_primary = -1;
        int shortest_secondary = -1;

        for (int primary = 0; primary < num_lines; primary++) {
            for (int secondary = 0; secondary < num_lines; secondary++) {
                double distance = 0;

                if (primary == secondary) {
                    continue;
                }

                if (primary > secondary) {
                    distance = distance_table[primary][secondary];
                } else {
                    distance = distance_table[primary][secondary];
                }

                if (distance < shortest_distance && distance > 0) {
                    shortest_distance = distance;
                    shortest_primary = primary;
                    shortest_secondary = secondary;
                }
            }
        }

        distance_table[shortest_primary][shortest_secondary] *= -1;

        _print_point(&points[shortest_primary]);
        printf(" -> ");
        _print_point(&points[shortest_secondary]);
        printf("\n");

        printf("Shortest Distance: p1: %d, p2: %d, d: %f\n", shortest_primary, shortest_secondary, shortest_distance);
    }

    printf("\n");

    _print_distance_table(distance_table, num_lines);

    printf("\n");

    for(int i = 0; i < num_lines; i++) {
        int prev_idx = -1;

        if (closest_idx[i] == INT_MIN) {
            printf("- ");
        } else {
            printf("%d ", closest_idx[i]);
        }
    }
    printf("\n");

    free(points);
    free(closest_idx);

    return 0;
}