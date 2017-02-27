#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "graph.h"

// num == -1 means print all
void print_edges(struct edge* head) {
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

int main(int argc, char *argv[] ) {
    /* deal with command line arguments */
    if (argc < 5) {
        printf("Not enough arguments, try again\n");
        return 1;
    }
    int prune, verbose;
    int flag = atoi(argv[1]);
    if (flag == 0) {
        prune = 1;
        verbose = 0;
    }
    else if (flag == 1) {
        prune = 1;
        verbose = 1;
    }
    else if (flag == 2) {
        prune = 0;
        verbose = 1;
    }
    int numpoints = atoi(argv[2]);
    if (numpoints < 8) {
        printf("Not enough points, try again\n");
        return 1;
    }
    int numtrials = atoi(argv[3]);
    int dimension = atoi(argv[4]);
    int type;
    switch (dimension) {
        case 0:
            type = RAND_WEIGHT;            
            break;
        case 2:
            type = RAND_COORD2;
            break;
        case 3:
            type = RAND_COORD3;
            break;
        case 4:
            type = RAND_COORD4;
            break;
        default:
            printf("Bad dimension argument, try again\n");
            return 1;
    }

    double weight, average_weight;
    struct timeval tval_before, tval_after, tval_result;
    struct graph *g;
    for (int t = 0; t < numtrials; t++) {
        /* timing */
        if (verbose) printf("TRIAL %d:\n", t + 1);
        gettimeofday(&tval_before, NULL);
        
        g = generate(type, numpoints, prune);
        if (verbose) printf("successfully generated graph\n");
        
        weight = mst_weight(g, verbose);
        if (verbose) printf("weight of MST: %f\n", weight);
        average_weight += weight;
        
        gettimeofday(&tval_after, NULL);
        timersub(&tval_after, &tval_before, &tval_result);
    
        if (verbose)
            printf("time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, 
                (long int)tval_result.tv_usec);

        if (verbose) printf("\n");

        free_graph(g);
    }
    average_weight /= (double) numtrials;

    printf("%f %d %d %d\n", average_weight, numpoints, numtrials, dimension);
    return 0;
}

