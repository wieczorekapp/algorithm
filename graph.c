#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "fifo.h" // koljka do bfs

#ifdef ADJ_MATRIX
    #include "adjacency_matrix.h"
#elif EDGE_LIST
    #include "edge_list.h"
#else
    #error "Compile with -DADJ_MATRIX or -DEDGE_LIST"
#endif

int ns[] = { 400, 800, 1600, 3200, 6400, 12800, 25600 /* TODO: set this to instances' sizes */ };
double bs[] = { 0.125, 0.375, 0.5, 0.625, 0.875 };

// fill graph with edges according to desired density
void graph_fill_edges(struct graph *g, double density) {
    // `t` is a helper array
    int *t = malloc(g->size * sizeof(int));

    // initialize `t` in increasing order
    for (int i = 0; i < g->size; i++) {
        t[i] = i;
    }

    // shuffle `t` to have random order of elements 0..n-1
    for (int i = g->size - 1; i > 0; i--) {
        int j = rand() & i;
        int tmp = t[i];
        t[i] = t[j];
        t[j] = tmp;
    }

    // create a path through all vertices in a random order
    for (int i = 1; i < g->size; i++) {
        graph_add_edge(g, t[i], t[i - 1]);
        graph_add_edge(g, t[i - 1], t[i]);
    }

    free(t);

    // total number of edges
    unsigned int edges = density * g->size * (g->size - 1) / 2.0;

    // randomly create new edges so that the required number is present
    for (int i = g->size - 1; i < edges; i++) {
        int x, y;
        do {
            x = rand() % g->size;
            y = rand() % g->size;
        } while (x == y || graph_has_edge(g, x, y));
        graph_add_edge(g, x, y);
        graph_add_edge(g, y, x);
    }
}

// check graph for correctness
void graph_check(struct graph *g, double density) {
    unsigned int actual_edges = 0;

    // check all n^2 pairs (including self-pairs) and if `i` has an edge to `j`, then `j` must have edge to `i`
    for (int i = 0; i < g->size; i++) {
        for (int j = 0; j < g->size; j++) {
            bool edge_ij = graph_has_edge(g, i, j);
            bool edge_ji = graph_has_edge(g, j, i);
            if (edge_ij != edge_ji) {
                error(EXIT_FAILURE, 0, "Inconsistent edge information: %d <-> %d is %s, but %d <-> %d is %s", i, j, edge_ij ? "true" : "false", j, i, edge_ji ? "true" : "false");
            }
            if (j > i && edge_ij) {
                actual_edges++;
            }
        }
    }

    // check actual edge count against expected one
    unsigned int expected_edges = density * g->size * (g->size - 1) / 2.0;
    if (actual_edges != expected_edges) {
        error(EXIT_FAILURE, 0, "Invalid edge count: actual value is %u, but expected is %u", actual_edges, expected_edges);
    }
}

int main(int argc, char **argv) {
    // `bs` contains densities of graphs
    for (int j = 0; j < sizeof(bs) / sizeof(*bs); j++) {
        double b = bs[j];

        // `ns` contains number of vertices
        for (int i = 0; i < sizeof(ns) / sizeof(*ns); i++) {
            int n = ns[i];

            // initialize random seed so that adjacency matrix and edge lists graphs are built randomly, but in a repeated way
            srand(i);

            // measure time of graph creation and filling it with edges
            clock_t graph_time = clock();

            struct graph *g = graph_create(n);
            if (g == NULL) {
                error(EXIT_FAILURE, 0, "Graph creation resulted in NULL");
            }

            graph_fill_edges(g, b);
            graph_time = clock() - graph_time;
            //printf("1\n");

            // check if edges were added correctly
            graph_check(g, b);

            //printf("2\n");
            //printf("3\n");

            // dfs
            int *visited = (int*) calloc(n, sizeof(int));
            clock_t dfs_time = clock();
            dfs(g, 0, visited);
            dfs_time = clock() - dfs_time;

            // check if dfs visited every vertex
            for (int k = 0; k < n; k++) {
                if (visited[k] == 0) {
                    error(EXIT_FAILURE, 0, "DFS implementation did not visit vertex: %u", k);
                }
            }
            //printf("3\n");
            // bfs
            memset(visited, 0, n * sizeof(int));
            clock_t bfs_time = clock();
            bfs(g, visited);
            bfs_time = clock() - bfs_time;

            // check if bfs visited every vertex
            for (int k = 0; k < n; k++) {
                if (visited[k] == 0) {
                    error(EXIT_FAILURE, 0, "BFS implementation did not visit vertex: %u", k);
                }
            }

            free(visited);

            unsigned int edges = b * g->size * (g->size - 1) / 2.0;
            printf("%-8g %-8u %-10u %-8g %-8g %-8g\n", b, n, edges,
                    (double)graph_time / CLOCKS_PER_SEC,
                    (double)dfs_time / CLOCKS_PER_SEC,
                    (double)bfs_time / CLOCKS_PER_SEC);

            graph_destroy(g);
        }
    }

    return 0;
}
