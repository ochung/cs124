#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "graph.h"

static struct edge *merge_lists(struct edge *e1, struct edge *e2);
struct edge *sort_by_weight(struct edge *head, int num);
static struct vertex *find_set(struct vertex *v);
static void path_compress_set(struct vertex *v, struct vertex *root);
static void union_set(struct vertex *v1, struct vertex *v2);
struct tree_edge *init_tree_edge(struct edge *e);
static void free_tree_list(struct tree_edge *head);

double mst_weight(struct graph *g) {
    g->edge_head = sort_by_weight(g->edge_head, g->n_edges);
    //printf("sorted edges by weight\n");

    struct edge *cur_edge = g->edge_head;
    struct tree_edge *tree_head = NULL;
    struct tree_edge *cur_tree_edge;
    for (long i = 0; i < g->n_edges; i++) {
        ////printf("iteration\n");
        if (find_set(cur_edge->source) != find_set(cur_edge->dest)) {
            union_set(cur_edge->source, cur_edge->dest);
            ////printf("did a union\n");

            if (!tree_head) {
                //printf("don't have a tree head\n");
                tree_head = init_tree_edge(cur_edge);
                cur_tree_edge = tree_head;
            } else {
                //printf("do have a tree head\n");
                cur_tree_edge->next_edge = init_tree_edge(cur_edge);
                cur_tree_edge = cur_tree_edge->next_edge;
            }
        }
        //printf("current %p, next edge is %p\n", cur_edge, cur_edge->next_edge);
        cur_edge = cur_edge->next_edge;
        //printf("before check %p\n", cur_edge);
        if (!cur_edge) break;
        //printf("assigned edge\n");
    }
    //printf("outside of loop\n");

    double total_weight = 0;
    cur_tree_edge = tree_head;
    //printf("set cur_tree to %p\n", tree_head);
    while (cur_tree_edge) {
        //printf("checking weight of %p edge %p\n", cur_tree_edge, cur_tree_edge->edge);
        total_weight += get_weight(cur_tree_edge->edge);        
        //printf("set new cur_tree_edge %p, next %p\n", cur_tree_edge, cur_tree_edge->next_edge);
        cur_tree_edge = cur_tree_edge->next_edge;
    }
    //printf("about to free %p\n", tree_head);
    
    free_tree_list(tree_head);

    return total_weight;
}

/* find the set of a given vertex */
static struct vertex *find_set(struct vertex *v) {
    struct vertex *cur_v = v;
    while (cur_v->set) {
        cur_v = cur_v->set;
    }
    return cur_v;
}

/* compress paths for a set given the root */
static void path_compress_set(struct vertex *v, struct vertex *root) {
    struct vertex *cur_v = v;
    struct vertex *next_v;
    
    while(cur_v->set) {
        next_v = cur_v->set;
        cur_v->set = root;
        cur_v = next_v;
    }
}

/* put together two sets */
static void union_set(struct vertex *v1, struct vertex *v2) {
    struct vertex *root1, *root2;
    root1 = find_set(v1);
    root2 = find_set(v2);
    if (root1->set_rank > root2->set_rank) {
        root2->set_rank = root1->set_rank;
        root1->set = root2;
    } else if (root1->set_rank > root2->set_rank) {
        root1->set_rank = root2->set_rank;
        root2->set = root1;
    } else {
        root1->set_rank++;
        root2->set = root1;
    }
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

/* subroutine for sorting */
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
            //printf("BAD EDGE TYPE, ABORTING\n");
            assert(0);
    }            
    return 0;
}

struct tree_edge *init_tree_edge(struct edge *e) {
    struct tree_edge *t = (struct tree_edge *) malloc(sizeof(struct tree_edge));
    t->edge = e;
    t->next_edge = NULL;
    return t;
}

static void free_tree_list(struct tree_edge *head) {
    struct tree_edge *cur_edge = head;
    struct tree_edge *next_edge;
    while (cur_edge) {
        next_edge = cur_edge->next_edge;
        free(cur_edge);
        cur_edge = next_edge;
    }
}
