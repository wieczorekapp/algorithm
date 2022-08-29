#ifndef __STACK_H__
#define __STACK_H__

struct stack {
    int index;
    int size;
    int *elements;
};

struct stack *stack_create(int size) {
    struct stack *s = malloc(sizeof(struct stack));
    s->index = 0;
    s->size = size;
    s->elements = malloc(size * sizeof(int));
    return s;
}

void stack_destroy(struct stack *s) {
    free(s->elements);
    free(s);
}

void stack_push(struct stack *s, int value) {
    assert(s->index < s->size);
    s->elements[s->index] = value;
    s->index++;
}

int stack_pop(struct stack *s) {
    s->index--;
    assert(s->index >= 0);
    return s->elements[s->index];
}

int stack_peek(struct stack *s) {
    assert(s->index - 1 >= 0);
    return s->elements[s->index - 1];
}

bool stack_is_empty(struct stack *s) {
    return s->index == 0;
}

#endif
