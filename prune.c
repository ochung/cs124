#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"

int prune_weight(int type, double weight, long n_vert) {
    switch(type) {
        case RAND_WEIGHT:
            if (weight > ((double) PRUNE_THRESH_0)/ ((double) n_vert))
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

double graph_threshold(struct tree_edge *head, int g_edges, int resolution) {
    struct tree_edge *cur_tree_edge = head;
    int *buckets = (int *) malloc(sizeof(int) * resolution);
    int tree_num = 0;
    while (cur_tree_edge) {
        for (int i = 0; i < resolution; i++) {
            if (get_weight(cur_tree_edge->edge) >= 
                    ((double) i / (double) resolution)) 
            {
                buckets[i]++;            
            }
        }
        tree_num++;
        cur_tree_edge = cur_tree_edge->next_edge;
    }

    double prob;
    for (int i = 0; i < resolution; i++) {
        prob = ((double) buckets[i] / (double) g_edges); 
        if (prob < THRESHOLD) {
            return ((double) i) / ((double) resolution);
        }
    } 
    return 1;
}

void threshold_test(int type, int resolution, int repetitions) {
    double thresh;
    struct graph *g; 
    for (int i = 10; i < 1000; i += 10) {
        thresh = 0;
        for (int j = 0; j < repetitions; j++) {
            g = generate(type, i, 0);
            mst_weight(g, 1);
            thresh += graph_threshold(g->tree_head, g->n_edges, resolution);
            free_graph(g);
        }
        printf("Threshold for %d points at weight %.20f\n", i, 
            ((double) thresh) / ((double) repetitions));
    }
}
