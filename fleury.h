#ifndef __FLEURY_H__
#define __FLEURY_H__

//memset(visited, 0, n * sizeof(int))
bool bridge(struct graph *g, int u, int v){
    int size_1 = 0;
    int size_2 = 0;
    int *dfs_1 = calloc(g->size, sizeof(int));
    int *dfs_2 = calloc(g->size, sizeof(int));
    dfs(g, u, dfs_1);
    for(int i = 0; i < g->size; i++){
        if(dfs_1[i] != 0){
            size_1++;
        }
    }

    graph_remove_edge(g, u, v);

    dfs(g, u, dfs_2);
    for(int i = 0; i < g->size; i++){
        if(dfs_2[i] != 0){
            size_2++;
        }
    }

    graph_add_edge(g, u, v);

    free(dfs_1);
    free(dfs_2);

    if(size_1 == size_2){
        return false;
    } else {
        return true;
    }
}

void euler(struct graph *g, int *cycle) {
    // TODO: implement this
    struct graph *tmp = graph_copy(g);
    bool bridge_v = true;
    int is_bridge_index = -1;
    int result_size = 0;

    int u = 0;
    int v = 0;

    cycle[result_size] = u;
    result_size++;    

    while(tmp->edges){
        for(v = 0; v < tmp->size; v++){
            if(graph_has_edge(tmp, u, v)){
                bridge_v = bridge(tmp, u, v);

                if(!bridge_v){
                    graph_remove_edge(tmp, u, v);
                    cycle[result_size] = v;
                    result_size++;
                    u = v;
                    is_bridge_index = -1;

                    break;
                } else {
                    if(is_bridge_index == -1){
                        is_bridge_index = v;
                    }
                }
            }
        }
        if(v == tmp->size){
            graph_remove_edge(tmp, u, is_bridge_index);
            cycle[result_size] = is_bridge_index;
            result_size++;
            u = is_bridge_index;
            is_bridge_index = -1;
        }
    }
    graph_destroy(tmp);
}

#endif
