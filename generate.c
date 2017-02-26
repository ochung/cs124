#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "graph.h"

/* generate a random graph of a given type with n nodes */
struct graph *generate(int type, long n) {
    // make sure that the type is correct
    assert(type == RAND_WEIGHT || type == RAND_COORD2 || 
        type == RAND_COORD3 || type == RAND_COORD4);
    (void) type; (void) n;
    return NULL;
}

struct edge *testedges(int num) {
    srand((unsigned) time(NULL));
    struct edge *head = calloc(1, sizeof(struct edge));
    struct edge *cur = head;
    for (int i = 0; i < num; i++) {
        cur->next_edge = calloc(1, sizeof(struct edge));
        cur->weight = ((double) rand()/(double) RAND_MAX);
        cur->type = RAND_WEIGHT;
        cur = cur->next_edge;
    }
    cur->weight = ((double) rand()/(double) RAND_MAX);
    cur->type = RAND_WEIGHT;
    return head;
}
