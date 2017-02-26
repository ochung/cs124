#include <stdio.h>
#include "graph.h"

// num == -1 means print all
void print_edges(struct edge* head, int type) {
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
    int num = 10000;
    struct edge *head = testedges(num);
    printf("UNSORTED \n");
    //print_edges(head, RAND_WEIGHT);

    head = sort_by_weight(head, num);

    printf("SORTED \n");
    //print_edges(head, RAND_WEIGHT);
    
    return 0;
}
