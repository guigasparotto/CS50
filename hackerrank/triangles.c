#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Small Triangles, Large Triangles
// https://www.hackerrank.com/challenges/small-triangles-large-triangles/problem

struct triangle
{
    int a;
    int b;
    int c;
};
typedef struct triangle triangle;

double area(struct triangle tr) {
    double p = (tr.a + tr.b + tr.c) / 2.0;
    return sqrt(p * (p - tr.a) * (p - tr.b) * (p - tr.c));
}

void sort_by_area(triangle* tr, int n) {
    for (int i = 0; i < n; i++) {
        int swaps = 0;

        for (int j = 0; j < n - i - 1; j++) {
            triangle t1 = tr[j];
            double s1 = area(t1);

            triangle t2 = tr[j + 1];
            double s2 = area(t2);

            if (s1 > s2) {
                tr[j] = t2;
                tr[j + 1] = t1;
                swaps++;
            }
        }

        if (swaps == 0) {
            break;
        }
    }
}

int main()
{
    int n;
    scanf("%d", &n);
    triangle *tr = malloc(n * sizeof(triangle));
    for (int i = 0; i < n; i++) {
        scanf("%d%d%d", &tr[i].a, &tr[i].b, &tr[i].c);
    }
    sort_by_area(tr, n);
    for (int i = 0; i < n; i++) {
        printf("%d %d %d\n", tr[i].a, tr[i].b, tr[i].c);
    }
    return 0;
}