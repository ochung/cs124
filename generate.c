#include <stdlib.h>
#include <assert.h>
#include "graph.h"

/* generate a random graph of a given type with n nodes */
struct graph *generate(int type, long n) {
    // make sure that the type is correct
    assert(type == RAND_WEIGHT || type == RAND_COORD2 || 
        type == RAND_COORD3 || type == RAND_COORD4);
    return NULL;
}


