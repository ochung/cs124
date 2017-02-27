#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"

int prune_weight(int type, double weight, long n_vert) {
    switch(type) {
        case RAND_WEIGHT:
            //if (weight > ((double) PRUNE_THRESH_0)/ ((double) n_vert))
            if (weight > .3)
                return 1;
            break;
        case RAND_COORD2:
            if (weight > ((double) PRUNE_THRESH_2)/ ((double) n_vert))
                return 1;
            break;
        case RAND_COORD3:
            if (weight > ((double) PRUNE_THRESH_3)/ ((double) n_vert))
                return 1;
            break;
        case RAND_COORD4:
            if (weight > ((double) PRUNE_THRESH_4)/ ((double) n_vert) + 
                    (double) PRUNE_OFF_4)
                return 1;
            break;
        default:
            assert(0);
            break;
    }
    return 0;
}

/* return value above which weights have prob < THRESHOLD of being in the MST */
double graph_threshold(struct tree_edge *head, int g_edges, int resolution) {
    /* initialize the buckets */
    int *buckets = (int *) malloc(sizeof(int) * resolution);
    
    /* number of edges in our tree */
    int tree_num = 0;

    /* traverse the entire list */
    struct tree_edge *cur_tree_edge = head;
    while (cur_tree_edge) {
        /* check whether this edge fits into each bucket */
        for (int i = 0; i < resolution; i++) {
            if (get_weight(cur_tree_edge->edge) >= 
                    ((double) i / (double) resolution)) 
            {
                buckets[i]++;            
            }
        }
        /* sum up the number of edges in the tree */
        tree_num++;
        cur_tree_edge = cur_tree_edge->next_edge;
    }

    /* find the correct weight threshold */
    double prob;
    for (int i = 0; i < resolution; i++) {
        prob = ((double) buckets[i]) / ((double) g_edges); 

        /* return the first one that is less than THRESHOLD */
        if (prob < THRESHOLD) {
            return ((double) i) / ((double) resolution);
        }
    } 
    return 1;
}

/* run a thresholding experiment for use in determining pruning */
void threshold_test(int type, int resolution, int repetitions, int verbose) {
    double thresh;
    struct graph *g; 
    for (int i = 10; i < 1000; i += 10) {
        thresh = 0;
        for (int j = 0; j < repetitions; j++) {
            /* generate a graph */
            g = generate(type, i, 0);

            /* find the maximum spanning tree weight */
            mst_weight(g, verbose);

            /* add up the thresholds */
            thresh += graph_threshold(g->tree_head, g->n_edges, resolution);
        
            /* clean up the graph we built */
            free_graph(g);
        }
        printf("Weight threshold for %d vertices at weight %.20f\n", i, 
            ((double) thresh) / ((double) repetitions));
    }
}
