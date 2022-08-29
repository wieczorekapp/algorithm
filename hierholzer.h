#ifndef __HIERHOLZER_H__
#define __HIERHOLZER_H__

void euler(struct graph *g, int *cycle) {
    // TODO: implement this
    struct stack *result = stack_create(g->edges + 1);
    struct graph *tmp = graph_copy(g);

    int result_size = 0;
    int u = 0;
    int v = 0;

    stack_push(result, 0);

    while(!stack_is_empty(result)){
        u = stack_peek(result);
        for(v = 0; v < tmp->size; v++){
            if(graph_has_edge(tmp, u, v)){
                graph_remove_edge(tmp, u, v);
                stack_push(result, v);
                u = v;
                break;
            }
        }

        if(v == tmp->size){
            cycle[result_size] = stack_pop(result);
            result_size++;
        }

    }
    graph_destroy(tmp);
    stack_destroy(result);
}

#endif
