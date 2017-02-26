/* 
 * Header file for programming assignment 1, CS 124, Spring 2017
 * Carl Denton and Olivia Chung
 */

#ifndef _GRAPHDEF
#define _GRAPHDEF 1
#define RAND_WEIGHT 0
#define RAND_COORD2 1
#define RAND_COORD3 2
#define RAND_COORD4 3

/* vertex struct */
struct vertex {
    /* which kind of graph is this? */
    int type; 

    /* 
     * set pointer is
     * NULL if we are the root 
     * otherwise points to some other node
     *
     * set rank is only meaningful at root
     */
    struct vertex *set;
    int set_rank;

    /* linked list for storage */
    struct vertex *next_vert;

    /* coordinates for parts 2 and 3 */
    int x; 
    int y;
    int z;
    int a;
};

/* edge struct */
struct edge {
    int type;
    struct edge *next_edge;
    struct vertex *source;
    struct vertex *dest;
    double weight;
};

struct tree_edge {
    struct edge *edge;    
    struct tree_edge *next_edge;
};

/* graph struct */
struct graph {
    /* number of nodes in this graph */
    long n_vert;
    long n_edges;

    /* point to our edge list */
    struct vertex *vert_head;
    struct edge *edge_head;
};

/* generate a graph */
struct graph *generate(int type, long n);
void free_graph(struct graph *g);
struct edge *testedges(int num);

/* find the weight of the minimum spanning tree on a graph */
double mst_weight(struct graph *g);

/* set operations for kruskal's algorithm */
void set_union(struct vertex *v1, struct vertex *v2);
void set_find(struct vertex *v);

/* utilities */
struct edge *sort_by_weight(struct edge *head, int num);
double get_weight(struct edge *e);
void print_edges(struct edge* head);

#endif
