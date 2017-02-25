#include <stdio.h>
#include <time.h>
#include "graph.h"
#define MAX_N 131072

int n = 4;

void insert(edge *e, int s, int d, int w)
{
    e->source = s;
    e->dest = d;
    e->weight = w;
}

long factorial(int a)
{
  int c;
  long result = 1;
 
  for (c = 1; c <= a; c++)
    result = result * c;
 
  return result;
}

int main (void)
{
    // srand(time(NULL));
    printf("factorial n is %ld\n", factorial(n));
    
    int edgenum = ((factorial(n)) / (factorial(2) * factorial(n - 2)));
    printf("edgenum is %i\n", edgenum);
    
    edge edgelist[edgenum];

    
    for (int i = 0; i < n; i++)
    {
        for (int j = i; j < n; j++)
        {
            if (i != j)
            {
                insert(&edgelist[i + j], i,  j, 1);
            }
        }
    }
    
    /*
    insert(&edgelist[0], 0, 1, 1);
    insert(&edgelist[1], 0, 2, 1);
    insert(&edgelist[2], 0, 3, 1);
    insert(&edgelist[3], 1, 2, 1);
    insert(&edgelist[4], 1, 3, 1);
    insert(&edgelist[5], 2, 3, 1);
    */
    
    for (int i = 0; i < edgenum; i++)
    {
        printf("source: %i, dest: %i, weight: %i\n", edgelist[i].source, edgelist[i].dest, edgelist[i].weight);
    }
}

/*
// nodes for adjacency list
typedef struct  {
    int val;
    struct AdjListNode* next;
} AdjListNode;

// graph structure
typedef struct 
{
    int num_vertices;
    struct AdjList* array;
} graph;

int n = 3;

int main ()
{
    AdjListNode *adj_list[n];    

    for (int i = 0; i < n; i++) {
         *adj_list[i] = NULL;
    }

}
*/
