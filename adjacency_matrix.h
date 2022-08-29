#ifndef _ADJACENCY_MATRIX_H_
#define _ADJACENCY_MATRIX_H_



struct graph {
    size_t size;                // number of vertices
    int **matrix;               // adjacency matrix
};

struct graph *graph_create(size_t size) {
    // TODO: implement this
    struct graph *tmp;

    tmp = malloc(1 * sizeof(struct graph));
    tmp->size = size;
    tmp->matrix = calloc(size, sizeof(int*));
    
    //printf("1C\n");
    for(int i = 0; i < size; i++){
        tmp->matrix[i] = calloc(size, sizeof(int));
    }
    //printf("2C\n");
    return tmp;
}

void graph_destroy(struct graph *g) {
    // TODO: implement this
    for(int i = 0; i < g->size; i++){
        free(g->matrix[i]);
    }
    free(g->matrix);
    free(g);
}

void graph_add_edge(struct graph *g, int i, int j) {
    // TODO: implement this
    g->matrix[i][j] = 1;
}

bool graph_has_edge(struct graph *g, int i, int j) {
    // TODO: implement this
    if(g->matrix[i][j] == 1){
        return true;
    } else {
        return false;
    }
}

void dfs(struct graph *g, int i, int *visited) {
    // TODO: implement this
    //printf("DFS_S\n");
    visited[i] = 1;
    for(int j = 0; j < g->size; j++){
        if(g->matrix[i][j] == 1 && !visited[j]){
            dfs(g, j, visited);
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

        for(int j = 0; j < g->size; j++){
            if(g->matrix[i][j] == 1 && !visited[j]){
                fifo_enqueue(queue, j);
                visited[j] = 1;
            }
        }
    }
    fifo_destroy(queue);
}

#endif
