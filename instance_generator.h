#ifndef __INSTANCE_GENERATOR_H__
#define __INSTANCE_GENERATOR_H__

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

void shuffle(int *t, int n) {
    for (int i = n - 1; i > 1; i--) {
        int j = rand() % i;
        int tmp = t[i];
        t[i] = t[j];
        t[j] = tmp;
    }
}

bool add_three_edges(struct graph *g, int *t) {
    shuffle(t, g->size);

    for (int x = 0; x < g->size; x++) {
        for (int y = x + 1; y < g->size; y++) {
            if (!graph_has_edge(g, x, y)) {
                for (int z = y + 1; z < g->size; z++) {
                    if (!graph_has_edge(g, x, z) && !graph_has_edge(g, y, z)) {
                        graph_add_edge(g, x, y);
                        graph_add_edge(g, x, z);
                        graph_add_edge(g, y, z);
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

struct graph *generate(int vertex_count, float density) {
    struct graph *g = graph_create(vertex_count);

    // randomly shuffled list of vertices
    int *t = malloc(vertex_count * sizeof(int));
    for (int i = 0; i < vertex_count; i++) {
        t[i] = i;
    }
    shuffle(t, vertex_count);

    // create hamiltionian cycle
    for (int i = 1; i < vertex_count; i++) {
        int j = t[i - 1];
        int k = t[i];
        graph_add_edge(g, j, k);
    }
    int first = t[0];
    int last = t[vertex_count - 1];
    graph_add_edge(g, first, last);

    // fill up edges maintaining eulerian cycle requirement
    int edges = density * vertex_count * (vertex_count - 1) / 2;
    for (int i = vertex_count; i < edges; i += 3) {
        if (!add_three_edges(g, t)) {
            break;
        }
    }

    free(t);
    return g;
}

#endif
