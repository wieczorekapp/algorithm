#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

unsigned int ns[] = { 10, /* TODO: fill values which will be used as lists' sizes */ };

// each tree node contains an integer key and pointers to left and right children nodes
struct node {
    int key;
    struct node *left;
    struct node *right;
};

// tree's beginning is called the root
struct node *root = NULL;

struct node **tree_search(struct node **candidate, int value) {
    struct node **result = candidate;

    if((*candidate)->key == value){
        result = candidate;
        return result;
    }

    if(value < (*candidate)->key){
        if((*candidate)->left != NULL){
            result = tree_search(&((*candidate)->left), value);
        } else {
        result = candidate;
        return result;
        }
    }

    if(value > (*candidate)->key){
        if((*candidate)->left != NULL){
        result = tree_search(&((*candidate)->right), value);
    }

    return result;
}


struct node* tree_insert(int value) {
    struct node **nodeptr = tree_search(&root, value);

    new_el = malloc(size(struct node));
    new_el->key = value;

    *nodeptr = struct node

    return *nodeptr;
}



struct node **tree_maximum(struct node **candidate) {
    // TODO: implement
    return NULL;
}

void tree_delete(int value) {
    // TODO: implement
}

unsigned int tree_size(struct node *element) {
    // TODO: implement
    return 0;
}

/*
 * Fill an array with increasing values.
 *
 * Parameters:
 *      int *t:     pointer to the array
 *      int n:      number of elements in the array
 */
void fill_increasing(int *t, int n) {
    for (int i = 0; i < n; i++) {
        t[i] = i;
    }
}

/*
 * Reorder array elements in a random way.
 *
 * Parameters:
 *      int *t:     pointer to the array
 *      int n:      number of elements in the array
 */
void shuffle(int *t, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % i;
        int temp = t[i];
        t[i] = t[j];
        t[j] = temp;
    }
}

/*
 * Check if tree is a valid BST.
 *
 * Parameters:
 *      struct node *element:   pointer to node to be checked
 *
 * Returns:
 *      bool:                   true if subtree rooted in "element" is a BST
 */
bool is_bst(struct node *element) {
    // empty tree is a valid BST
    if (element == NULL) {
        return true;
    }
    // leaf node is valid
    if (element->left == NULL && element->right == NULL) {
        return true;
    }
    // only right subnode? check it recursively
    if (element->left == NULL && element->right != NULL) {
        return (element->key < element->right->key) && is_bst(element->right);
    }
    // only left subnode? check it recursively
    if (element->left != NULL && element->right == NULL) {
        return (element->key > element->left->key) && is_bst(element->left);
    }
    // both subnodes present? check both recursively
    return (element->key > element->left->key)
        && (element->key < element->right->key)
        && is_bst(element->left)
        && is_bst(element->right);
}

void insert_increasing(int *t, int n) {
    for (int i = 0; i < n; i++) {
        tree_insert(t[i]);
    }
}

void insert_random(int *t, int n) {
    shuffle(t, n);
    for (int i = 0; i < n; i++) {
        tree_insert(t[i]);
    }
}

//---------------------------------------------------------------------------
//SORTING ALGORITHM - QUICK SORT
//---------------------------------------------------------------------------
int partition(int *t, int p, int r)
{
    int x, i;
    x = t[r];
    i = p - 1;

    for(int j = p; j <= r - 1; j++)
    {
        if(t[j] < x)
        {
            i = i + 1;
            swap(t, i, j);
        }
    }

    i = i + 1;
    swap(t, i, r);

    return i;
}

int random_partition(int *t, int p, int r)
{
    int i = p + (rand() % (r - p));
    swap(t, i, r);
    return partition(t, p, r);

}

void quick_sort(int *t, int p, int r)
{
    int q;

    if(p < r)
    {
        //q = partition(t, p, r);
        q = random_partition(t, p, r);
        quick_sort(t, p, q - 1);
        quick_sort(t, q + 1, r);
    }
}

void quick_sort(int *t, unsigned int n)
{
    quick_sort(t, 0, n - 1);
}
//---------------------------------------------------------------------------
//SORTING ALGORITHM - QUICK SORT
//---------------------------------------------------------------------------

void insert_binary(int *t, unsigned int n) {
    quick_sort(t, 0, n - 1);
    insert_binary_bijection(*t, 0, n -1);
}

void insert_binary_bijection(int *t, int p, int r){
    int q;

    if(p == r){
        tree_insert(t[p]);
    } else if(r - p == 1) {
        tree_insert(t[p]);
        tree_insert(t[r]);
    } else {
        q = p + (r -p) / 2;
        tree_insert(t[q]); // dodanie el srodkowego
        insert_binary_bijection(t, p, q-1);
        insert_binary_bijection(t, q+1, r);

    }

}

char *insert_names[] = { "Increasing", "Random", "Binary" };
void (*insert_functions[])(int*, int) = { insert_increasing, insert_random, insert_binary };

int main(int argc, char **argv) {
    for (unsigned int i = 0; i < sizeof(insert_functions) / sizeof(*insert_functions); i++) {
        void (*insert)(int*, int) = insert_functions[i];

        for (unsigned int j = 0; j < sizeof(ns) / sizeof(*ns); j++) {
            unsigned int n = ns[j];

            // crate an array of increasing integers: 0, 1, 2, ...
            int *t = malloc(n * sizeof(*t));
            fill_increasing(t, n);

            // insert data using one of methods
            clock_t insertion_time = clock();
            insert(t, n);
            insertion_time = clock() - insertion_time;

            assert(tree_size(root) == n);       // after all insertions, tree size must be `n`
            assert(is_bst(root));               // after all insertions, tree must be valid BST

            // reorder array elements before searching
            shuffle(t, n);

            // search for every element in the order present in array `t`
            clock_t search_time = clock();
            for (unsigned int k = 0; k < n; k++) {
                struct node **pnode = tree_search(&root, t[k]);
                struct node *iter = *pnode;
                assert(iter != NULL);           // found element cannot be NULL
                assert(iter->key == t[k]);      // found element must contain the expected value
            }
            search_time = clock() - search_time;

            // reorder array elements before deletion
            shuffle(t, n);

            // delete every element in the order present in array `t`
            for (unsigned int l = 0, m = n; l < n; l++, m--) {
                assert(tree_size(root) == m);   // tree size must be equal to the expected value
                tree_delete(t[l]);
                assert(is_bst(root));           // after deletion, tree must still be valid BST
            }
            assert(tree_size(root) == 0);       // after all deletions, tree has size zero

            free(root);
            free(t);

            printf("%d\t%s\t%f\t%f\n",
                    n,
                    insert_names[i],
                    (double)insertion_time / CLOCKS_PER_SEC,
                    (double)search_time / CLOCKS_PER_SEC);
        }
    }
    return 0;
}
