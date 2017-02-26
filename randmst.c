#include <stdio.h>
#include "graph.h"

// num == -1 means print all
void print_edges(struct edge* head) {
    printf("in print_edges\n");
    struct edge *cur = head;

    double weight = 0;
    int counter = 0;
    while (cur) {
        weight = get_weight(cur);
        printf("edge %d: weight %f\n", counter, weight);

        cur = cur->next_edge;
        counter++;
    }
}

int main(void) {
    int num = 4;
    struct graph *g = generate(RAND_WEIGHT, num);
    printf("successfully generated graph\n");
    print_edges(g->edge_head);
    g->edge_head = sort_by_weight(g->edge_head, g->n_edges);
    printf("SORTED \n");
    print_edges(g->edge_head);
    
    return 0;
}
