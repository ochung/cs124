/* 
 * Header file for programming assignment 1, CS 124, Spring 2017
 * Carl Denton and Olivia Chung
 */

// edge struct
typedef struct edge {
    int source;
    int dest;
    float weight;
};

typedef struct graph {
    /* number of nodes in this graph */
    int n;

    struct edge* head;
};


void insert(struct graph*, edge *e, int source, int dest, float weight);
