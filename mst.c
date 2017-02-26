#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "graph.h"

static struct edge *merge_lists(struct edge *e1, struct edge *e2);
struct edge *sort_by_weight(struct edge *head, int num);

float mst_weight(struct graph *g) {
    return 0;
}

/* sorting subroutine for Kruskal's algorithm 
 * sorts elements of a linked list by increasing weight
 * implements mergesort
 */
struct edge *sort_by_weight(struct edge *head, int num) {
    /* base case, return identically, clearing out successor pointers */
    if (num == 1) {
        head->next_edge = NULL;
        return head;
    }
    /* split into two parts, sort each and merge */
    else {
        struct edge* e1 = head;
        struct edge* e2 = head;
        for (int i = 0; i < num/2; i++) {
            e2 = e2->next_edge;            
        }
        e1 = sort_by_weight(e1, num/2);
        e2 = sort_by_weight(e2, num - num/2);
        return merge_lists(e1, e2);
    }
    assert(0);
    return NULL;
}

static struct edge *merge_lists(struct edge *e1, struct edge *e2) {
    /* initialize helper variables */
    struct edge *head, *last, *next, *next_e1, *next_e2;
    head = last = NULL;
    next_e1 = e1;
    next_e2 = e2;

    /* step through both lists */
    while (next_e1 || next_e2) {
        /* take from list 1 if no option or it is better */
        if (next_e1 && (!next_e2 || get_weight(next_e1) < get_weight(next_e2))) {
            next = next_e1;
            next_e1 = next_e1->next_edge;
        }
        else {
            next = next_e2;
            next_e2 = next_e2->next_edge;
        }

        /* make a head if we don't have one, otherwise add to our list */
        if (!head) {
            head = next;
            last = next;
        }
        else {
            last->next_edge = next;
            last = last->next_edge;
        }
    }
    return head;
}

/* return the weight on an edge */
double get_weight(struct edge *e) {
    int type = e->type;
    switch(type) {
        case RAND_WEIGHT:
            return e->weight;        
        case RAND_COORD2:
            return pow(e->source->x - e->dest->x, 2)
                + pow(e->source->y - e->dest->y, 2);
        case RAND_COORD3:
            return pow(e->source->x - e->dest->x, 2)
                + pow(e->source->y - e->dest->y, 2)
                + pow(e->source->z - e->dest->z, 2);
        case RAND_COORD4: 
            return pow(e->source->x - e->dest->x, 2)
                + pow(e->source->y - e->dest->y, 2)
                + pow(e->source->z - e->dest->z, 2)
                + pow(e->source->a - e->dest->a, 2);
        default:
            printf("BAD EDGE TYPE, ABORTING\n");
            assert(0);
    }            
    return 0;
}
