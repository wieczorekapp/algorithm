#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "adjacency_matrix.h"
#include "instance_generator.h"
#include "stack.h"

//#define FLEURY 1
#define HIERHOLZER 1

#ifdef FLEURY
    #include "fleury.h"
#elif HIERHOLZER
    #include "hierholzer.h"
#endif

int vertex_count = 200;
double densities[] = {
    0.125, 0.1875, 0.25, 0.3125,
    0.375, 0.4375, 0.5,  0.5625,
    0.625, 0.6875, 0.75, 0.8125,
    0.875
};

void check_euler(struct graph *g, int *cycle) {
    int **matrix = malloc(g->size * sizeof(int *));
    for (int i = 0; i < g->size; i++) {
        matrix[i] = calloc(g->size, sizeof(int));
    }

    for (int i = 1; i < g->edges; i++) {
        int u = cycle[i - 1];
        int v = cycle[i];

        // make sure that (u,v) exists in the graph
        assert(graph_has_edge(g, u, v));

        // make sure that (u,v) appears only once
        assert(matrix[u][v] == 0);
        assert(matrix[v][u] == 0);
        matrix[u][v] = matrix[v][u] = 1;
    }

    // also check (first,last)
    int u = cycle[0];
    int v = cycle[g->edges - 1];
    assert(graph_has_edge(g, u, v));
    assert(matrix[u][v] == 0);
    assert(matrix[v][u] == 0);

    for (int i = 0; i < g->size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    srand(time(NULL));

    for (unsigned int i = 0; i < sizeof(densities) / sizeof(*densities); i++) {
        float density = densities[i];

        // generate instance
        struct graph *g = generate(vertex_count, density);

        // prepare memory for storing eulerian cycle
        int *cycle = calloc(g->edges + 1, sizeof(int));

        // create a copy of graph (so that edge removals are not permanent)
        struct graph *h = graph_copy(g);

        // find eulerian cycle
        clock_t euler_time = clock();
        euler(h, cycle);
        euler_time = clock() - euler_time;

        // check if it is correct
        check_euler(g, cycle);

        printf("%-8d %-8g\n", g->edges, (double)euler_time / CLOCKS_PER_SEC);

        free(cycle);
        graph_destroy(g);
        graph_destroy(h);
    }
    return 0;
}
