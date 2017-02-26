#include <stdlib.h>
#include <assert.h>
#include "graph.h"

/* generate a random graph of a given type with n nodes */
struct graph *generate(int type, long n) {
	
	srand(time(NULL));
	
    // make sure that the type is correct
    assert(type == RAND_WEIGHT || type == RAND_COORD2 || 
        type == RAND_COORD3 || type == RAND_COORD4);
   
    /* create linked list of n vertices */
    //struct vertex *v_head = NULL;
    struct vertex *v_head = (struct vertex *) malloc(sizeof(struct vertex)); 
    v_head = NULL;
    //struct vertex *curr_vertex = NULL;
    struct vertex *curr_vertex = (struct vertex *) malloc(sizeof(struct vertex)); 
    
    for (int i = 0; i < n; i++) {
    	
    	// create a node
    	//struct vertex *v = malloc(sizeof(v));
    	struct vertex *v = (struct vertex *) malloc(sizeof(struct vertex));
    	if (v == NULL) {
    		return NULL;
    	}
    	
    	// if the node is the first one in the linked list
    	if (v_head == NULL) {
    		v_head = v;
    		v->next_vert = NULL;
    		curr_vertex = v_head;
    		v->x = i;
    	}
    	
    	// if a linked list already exists
    	else {
    		curr_vertex->next_vert = v;
    		curr_vertex = v;
    		v->x = i;
    	}
    }
    
    // pointer to iterate through vertex LL (source)
    struct vertex *vert_source = (struct vertex *) malloc(sizeof(struct vertex)); 
    //struct vertex *vert_source = v_head;
    vert_source = v_head;
    
	// pointer to traverse through vertex LL (destination)
	struct vertex *vert_dest = (struct vertex *) malloc(sizeof(struct vertex)); 
    //struct vertex *vert_source = v_head;
    vert_dest = v_head->next_vert;
    //struct vertex *vert_dest = v_head->next_vert;
    
    struct edge *edge_head = (struct edge *) malloc(sizeof(struct edge)); 
    //struct edge *edge_head = NULL;
    edge_head = NULL;
    struct edge *curr_edge = (struct edge *) malloc(sizeof(struct edge)); 
    curr_edge = NULL;
    //struct edge *curr_edge = NULL;
    
    while (vert_source->next_vert != NULL) {
    	while (vert_dest != NULL){
    		
    		// create an edge from the source to the destination
    		struct edge *new_edge = (struct edge *) malloc(sizeof(new_edge));
    		//new_edge->weight = 18.0;
    		//printf("0 %f\n", edge_head->weight);
    		// if the edge LL is empty
	    	if (edge_head == NULL) {
	    		edge_head = new_edge;
	    		new_edge->next_edge = NULL;
	    		curr_edge = edge_head;
	    	}
	    	else {
	    		curr_edge->next_edge = new_edge;
	    		curr_edge = new_edge;
	    	}
	    	new_edge->weight = 18.0;
	    	new_edge->source = vert_source;
	   		new_edge->dest = vert_dest;
	   		vert_dest = vert_dest->next_vert;
    	}
    	
    	// move source and dest ptr
    	vert_source = vert_source->next_vert;
    	vert_dest = vert_source->next_vert;
    }
   
    // print out edge LL
    struct edge *temp = edge_head;
    while(temp != NULL)
    {
    	printf("%i ", temp->source->x);
    	printf("%i ", temp->dest->x);
    	printf("%f\n", temp->weight);
    	temp=temp->next_edge;
    }
    
   	struct graph *new_graph = malloc(sizeof(new_graph));
    if (new_graph == NULL) {
   		return NULL;
   	}
    new_graph->n = n;
    new_graph->head = edge_head;
    
    return new_graph;
}

