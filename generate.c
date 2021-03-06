#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "graph.h"

struct valloc {
    struct valloc *next_alloc;
    struct vertex *ptr;
};

struct alloc {
    struct alloc *next_alloc;
    struct edge *ptr;
};

static struct vertex *init_vert(struct vertex *ptr, int type, struct vertex *next_v);
static struct edge *init_edge(struct edge *ptr, double weight, struct vertex *source, 
    struct vertex *dest);
static double new_weight(int type, struct vertex *source, struct vertex *dest);
static void free_vertex_list(struct vertex *head);
static void free_edge_list(struct edge *head);
static void free_allocation_list(struct alloc *head);
static void free_vallocation_list(struct valloc *head);
void free_graph(struct graph *g);



/* generate a random graph of a given type with n nodes */
struct graph *generate(int type, long n_vert, int prune) {
    /* seed the random number generator */
    struct timeval tval;
    gettimeofday(&tval, NULL);
    srand((unsigned int) tval.tv_usec);

    /* create linked list of n_vert vertices */
    struct vertex *vert_head, *cur_vertex; 
    vert_head = cur_vertex = NULL; 
    
    /* set up the linked list of vertex block allocations */
    long vert_structs_allocated = 0;
    struct vertex *vptr;
    struct valloc *valloc_head = NULL;
    struct valloc *cur_valloc;

    /* make n_vert vertices */
    for (long i = 0; i < n_vert; i++) {
        /* if we're due for another block allocation, do that */
        if (vert_structs_allocated == 0) {
            /* allocate a block */
            vptr = (struct vertex *) malloc(MEM_BLOCK*sizeof(struct vertex));

            /* if the vertex allocation list doesn't have a head, make one */
            if (!valloc_head) {
                valloc_head = (struct valloc *) malloc(sizeof(struct valloc));
                cur_valloc = valloc_head;
            /* otherwise, put our allocation at the end of the list */
            } else {
                cur_valloc->next_alloc = (struct valloc *) 
                    malloc(sizeof(struct valloc));
                cur_valloc = cur_valloc->next_alloc;
            }
            cur_valloc->next_alloc = NULL;
            cur_valloc->ptr = vptr;
        }


        /* initialize a vertex with the preallocated memory */
        struct vertex *v = init_vert(vptr + vert_structs_allocated, type, NULL);
        
        /* failed initialization, clean up */
        if (!v) {
            free_vallocation_list(valloc_head);
            return NULL;
        }

        /* if we don't have a head for the vertex list, make this one the head */
        if (vert_head == NULL) {
            vert_head = v; 
            cur_vertex = vert_head;
        }

        /* if we do have a head, add the new vertex to the end */
        else {
            cur_vertex->next_vert = v;
            cur_vertex = v;
        }
        vert_structs_allocated = (vert_structs_allocated + 1) % MEM_BLOCK;
    }
    
    /* pointers for traversing vertex linked list */
    struct vertex *vert_source = vert_head;
    struct vertex *vert_dest = vert_head->next_vert;
    
    /* pointers for traversing edge linked list */
    struct edge *edge_head, *cur_edge; 
    edge_head = cur_edge = NULL;
    
    /* count the edges we add to the graph */ 
    long n_edges = 0;

    /* set up edge allocation linked list */
    long structs_allocated = 0;
    struct alloc *alloc_head = NULL;
    struct alloc *cur_alloc = NULL;
    struct edge *ptr = NULL;
    double weight;

    /* iterate over source nodes */
    while (vert_source->next_vert != NULL) {
        /* start right after the source vertex */
        vert_dest = vert_source->next_vert;
        while (vert_dest != NULL){
            /* create an edge from the source to the destination */
            weight = new_weight(type, vert_source, vert_dest);
            
            if (prune && prune_weight(type, weight, n_vert)) {
                /* move destination pointer */
                vert_dest = vert_dest->next_vert;
                continue;
            }
            
            if (structs_allocated == 0) {
                ptr = (struct edge *) malloc(MEM_BLOCK*sizeof(struct edge));
                if (!alloc_head) {
                    alloc_head = (struct alloc *) malloc(sizeof(struct alloc));
                    cur_alloc = alloc_head;
                    alloc_head->next_alloc = NULL;
                } else {
                    cur_alloc->next_alloc = (struct alloc *) malloc(sizeof(struct alloc));
                    cur_alloc = cur_alloc->next_alloc;
                    cur_alloc->next_alloc = NULL;
                }
                cur_alloc->ptr = ptr;
            }
            struct edge *new_edge = init_edge(cur_alloc->ptr + structs_allocated, 
                weight, vert_source, vert_dest);
        
            /* failed initialization, clean up */
            if (!new_edge) {
                free_vertex_list(vert_head);
                //free_edge_list(edge_head);
                free_allocation_list(alloc_head);
                return NULL;
            }

            /* if we don't have an edge head, make one */
            if (edge_head == NULL) {
                edge_head = new_edge;
                cur_edge = edge_head;
            }
            /* otherwise, add this edge at the end of the list */
            else {
                cur_edge->next_edge = new_edge;
                cur_edge = new_edge;
            }

            /* move destination pointer */
            vert_dest = vert_dest->next_vert;
            n_edges++;
            structs_allocated = (structs_allocated + 1) % MEM_BLOCK;
        }
        
        /* move source pointer */
        vert_source = vert_source->next_vert;
    }
   
    /* allocate space for the graph struct */
    struct graph *new_graph = malloc(sizeof(struct graph));

    /* failed allocation, clean up */
    if (!new_graph) {
        free_vertex_list(vert_head);
        //free_edge_list(edge_head);
        free_allocation_list(alloc_head);
        return NULL;
    }

    /* set fields */
    new_graph->n_vert = n_vert;
    new_graph->n_edges = n_edges;
    new_graph->vert_head = vert_head;
    new_graph->edge_head = edge_head;
    new_graph->tree_head = NULL;
    new_graph->alloc_head = alloc_head;
    new_graph->valloc_head = valloc_head;
    
    //printf("generated graph with %ld edges, %ld max\n", 
    //    n_edges, n_vert*(n_vert-1)/2);
    return new_graph;
}

/* generate a weight for an edge */
static double new_weight(int type, struct vertex *source, struct vertex *dest) {
    switch(type) {
        case RAND_WEIGHT:
            return ((double) rand()/(double) RAND_MAX);
        case RAND_COORD2:
            return sqrt(pow(source->x - dest->x, 2) + pow(source->y - dest->y, 2));
        case RAND_COORD3:
            return sqrt(pow(source->x - dest->x, 2) + pow(source->y - dest->y, 2)
                + pow(source->z - dest->z, 2));
        case RAND_COORD4:
            return sqrt(pow(source->x - dest->x, 2) + pow(source->y - dest->y, 2)
                + pow(source->z - dest->z, 2) + pow(source->a - dest->a, 2));
        default:
            /* failure, return garbage */
            return -1;
    }
}

/* initialize a vertex */
static struct vertex *init_vert(struct vertex *ptr, int type, struct vertex *next_v) {
    /* allocate space for the vertex */
    struct vertex *v;
    if (!ptr) 
        v = (struct vertex *) malloc(sizeof(struct vertex));
    else
        v = ptr;


    /* failed allocation */
    if (v == NULL) return NULL;
    
    /* set fields */
    v->type = type;
    v->set = NULL;
    v->next_vert = NULL;

    /* initialize coordinates */
    switch(type) {
        case RAND_WEIGHT:
            v->x = 0;
            v->y = 0;
            v->z = 0;
            v->a = 0;
            return v;
        case RAND_COORD2:
            v->x = ((double) rand()/(double) RAND_MAX);
            v->y = ((double) rand()/(double) RAND_MAX);
            v->z = 0;
            v->a = 0;
            return v;
        case RAND_COORD3:
            v->x = ((double) rand()/(double) RAND_MAX);
            v->y = ((double) rand()/(double) RAND_MAX);
            v->z = ((double) rand()/(double) RAND_MAX);
            v->a = 0;
            return v;
        case RAND_COORD4:
            v->x = ((double) rand()/(double) RAND_MAX);
            v->y = ((double) rand()/(double) RAND_MAX);
            v->z = ((double) rand()/(double) RAND_MAX);
            v->a = ((double) rand()/(double) RAND_MAX);
            return v;
        default:
            /* failure, free the vertex and get out */
            free(v);
            return NULL;
    }    
}

/* initialize an edge */
static struct edge *init_edge(struct edge *ptr, double weight, struct vertex *source, 
        struct vertex *dest) {
    assert(source->type == dest->type);

    /* allocate space for the edge */
    struct edge *new_edge;
    if (!ptr) 
        new_edge = (struct edge *) malloc(sizeof(struct edge));
    else
        new_edge = ptr;

    /* failed allocation */
    if (!new_edge) return NULL;

    /* set fields */
    new_edge->type = source->type;
    new_edge->next_edge = NULL;
    new_edge->source = source;
    new_edge->dest = dest;
    new_edge->weight = weight;
    return new_edge;
}

/* free memory for a vertex linked list */
static void free_vertex_list(struct vertex *head) {
    struct vertex *cur_vertex = head;
    struct vertex *next_vertex;
    while (cur_vertex) {
        next_vertex = cur_vertex->next_vert;
        free(cur_vertex);
        cur_vertex = next_vertex;
    }
}

/* free memory for an edge linked list */
static void free_edge_list(struct edge *head) {
    struct edge *cur_edge = head;
    struct edge *next_edge;
    while (cur_edge) {
        next_edge = cur_edge->next_edge;
        free(cur_edge);
        cur_edge = next_edge;
    }
}

static void free_allocation_list(struct alloc *head) {
    struct alloc* cur_alloc = head;
    struct alloc* next_alloc = cur_alloc;
    while (cur_alloc) {
        next_alloc = cur_alloc->next_alloc;        
        free(cur_alloc);
        cur_alloc = next_alloc;
    }
}

static void free_vallocation_list(struct valloc *head) {
    struct valloc* cur_valloc = head;
    struct valloc* next_valloc = cur_valloc;
    while (cur_valloc) {
        next_valloc = cur_valloc->next_alloc;        
        free(cur_valloc);
        cur_valloc = next_valloc;
    }
}


void free_graph(struct graph *graph) {
    free_tree_list(graph->tree_head);
    free_allocation_list(graph->alloc_head);
    free_vallocation_list(graph->valloc_head);
    free(graph);
}


/* initialize a bunch of edges (only for testing) */
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

