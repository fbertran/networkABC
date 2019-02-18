/**
* Represents collections of graphs. 
* Copyright (c) 2015 K.Musayeva <khmusayeva@gmail.com>
**/

#ifndef POOL_H
#define POOL_H

typedef struct pool_str {
	int size;
	Graph  **graphs;
} Pool;


typedef struct List_str {
        int index;
        Graph *motif;
        struct List_str *next;
        } List;

void createPoolof3Motifs(Pool *pool, int size, int precise_number_nodes, double clust_coef);
void cleanupPool(Pool *pool);
#endif


