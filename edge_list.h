#ifndef _EDGE_LIST_H_
#define _EDGE_LIST_H_

#include <stdbool.h>

struct edge_list {
    size_t size;                // number of edges
    int *vertices;              // adjacent vertices
};

struct graph {
    size_t size;                // number of vertices
    struct edge_list *edges;    // edge lists
};

struct graph *graph_create(size_t size) {
    // TODO: implement this
    struct graph *tmp_g;

    tmp_g = malloc(1 * sizeof(struct graph));
    tmp_g->size = size; 
    tmp_g->edges = malloc(size * sizeof(struct edge_list)); // * ?????

    for(int i = 0; i < size; i++){
        tmp_g->edges[i].vertices = malloc(size * sizeof(int));
        tmp_g->edges[i].size = 0;

        //printf("%d TEST %d\n", i, tmp_g->edges[i].size);
        //printf("%d\n", i);
    }
    return  tmp_g;
}

void graph_destroy(struct graph *g) {
    for(int i = 0; i < g->size; i++){
        free(g->edges[i].vertices);
    }
    free(g->edges);
    free(g);
}

void graph_add_edge(struct graph *g, int i, int j) {
    // TODO: implement this
    for(int x = 0; x < g->edges[i].size; x++){
        if(g->edges[i].vertices[x] == j){
            return;
        }
    }
    //printf("======================\n");
    //printf("i %d %d\n",i ,g->edges[i].size);
    g->edges[i].vertices[g->edges[i].size] = j;
    //printf("%d i indeks %d: %d\n", i, g->edges[i].size, g->edges[i].vertices[g->edges[i].size]);
    g->edges[i].size += 1;

    //printf("j %d %d\n",j ,g->edges[j].size);
    g->edges[j].vertices[g->edges[j].size] = i;
    //printf("%d j indeks %d: %d\n", j, g->edges[i].size, g->edges[j].vertices[g->edges[j].size]);
    g->edges[j].size += 1;
}

bool graph_has_edge(struct graph *g, int i, int j) {
    // TODO: implement this
    for(int x = 0; x < g->edges[i].size; x++){
        if(g->edges[i].vertices[x] == j){
            return true;
        }
    }
    return false;
}

//
void dfs(struct graph *g, int i, int *visited) {
    // TODO: implement this
    /*
    visited[i] = 1;
    for(int j = 0; j < g->size; j++){
        if(g->matrix[i][j] == 1 && !visited[j]){
            dfs(g, j, visited);
        }
    }*/
    visited[i] = 1;
    for(int j = 0; j < g->edges[i].size; j++){
        if(!visited[g->edges[i].vertices[j]]){
            dfs(g, g->edges[i].vertices[j], visited);
        }
    }
}

void bfs(struct graph *g, int *visited) {
    // TODO: implement this
    struct fifo *queue;
    int i;

    queue = fifo_create(g->size);
    fifo_enqueue(queue, 0);

    while(!fifo_is_empty(queue)){
        i = fifo_dequeue(queue);//zciagniecie wierzcholka z kolejki
        visited[i] = 1;// wierzcholek odwiedzony

        for(int j = 0; j < g->edges[i].size; j++){
            if(!visited[g->edges[i].vertices[j]]){
                fifo_enqueue(queue, g->edges[i].vertices[j]);
                visited[g->edges[i].vertices[j]] = 1;
            }
        }
    }
    fifo_destroy(queue);
}

#endif
