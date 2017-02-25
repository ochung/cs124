/* 
 * Header file for programming assignment 1, CS 124, Spring 2017
 * Carl Denton and Olivia Chung
 */

#define RAND_WEIGHT 0
#define RAND_COORD2 1
#define RAND_COORD3 2
#define RAND_COORD4 3

/* vertex struct */
struct vertex {
    /* which kind of graph is this? */
    int type; 

    /* 
     * NULL if we are the root 
     * otherwise points to some other node
     */
    struct vertex *set;

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
    struct edge *next_edge;
    struct vertex *source;
    struct vertex *dest;
    double weight;
};

/* graph struct */
struct graph {
    /* number of nodes in this graph */
    long n;

    /* point to our edge list */
    struct edge *head;
};

/* generate a graph */
struct graph *generate(int type, long n);

/* find the weight of the minimum spanning tree on a graph */
float mst_weight(struct graph *g);

/* set operations for kruskal's algorithm */
void set_union(struct vertex *v1, struct vertex *v2);
void set_find(struct vertex *v);

/* sorting subroutine for kruskal's algorithm */
struct edge *sort_by_weight(struct edge *head);
