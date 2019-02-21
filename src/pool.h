/**
 * Represents collections of graphs. 
 * 2015. Author K.Musayeva <khmusayeva@gmail.com>
 * 2019. Fixes and updates by F. Bertrand <fbertran@math.unistra.fr>
 * Copyright (c) Universite de Strasbourg and CNRS
 */

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


