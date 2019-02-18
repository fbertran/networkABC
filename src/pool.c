/**
 * This file provides the functionality of building a pool of motifs.
 * Copyright (c) 2015 K.Musayeva <khmusayeva@gmail.com>.
 */
#include "stdlib.h"
#include "graph.h"
#include "pool.h"
#include "motif.h"
#include "utils.h"
#define MIN_NODES 2
#define MAX_NODES 8
#define CM 0.3 //this is a clustering coefficient


/**
 * This function is for debugging.
 */
Graph *createMotif(int number_of_nodes){
 
	 Graph *graph;
 
	 int type=sample_int(1,3);	

	 if(number_of_nodes<3) {
                graph=generateSIM(number_of_nodes, 0);
                        }

         else {
		  switch(type) {
	                  case 1:
                             graph=generateSIM(number_of_nodes, 0);
                          break;
                          case 2:
                             graph=generateDOR(number_of_nodes, 0);
                          break;
                          case 3:
                             graph=generateFL(number_of_nodes, 0);
                          break;
                               }

                        }

	return graph;

	}



/**
 * This function is for debugging.
 */
void createPool(Pool *pool, int size, int precise_number_nodes) {
	//pool=malloc(sizeof(Pool));
	pool->size=size;
	pool->graphs=malloc(size*sizeof(Graph*));

	int i, type, max_nodes, max_regulators;
	//int types[]={1,2,3};

	Graph *graph;
	for(i=0;i<size;++i) {
		type=sample_int(1, 3);
		//type=types[i];

		max_nodes=(precise_number_nodes==0)?sample_int(MIN_NODES, MAX_NODES):precise_number_nodes;

		if(max_nodes<3) {
			graph=generateSIM(precise_number_nodes, max_nodes);
			}

		else {
        		switch(type) {
                		case 1:
	                		graph=generateSIM(precise_number_nodes, max_nodes);
	        	        break;
        	        	case 2:
		        	        graph=generateDOR(precise_number_nodes, max_nodes);
        	        	break;
	                	case 3:	
			                graph=generateFL(precise_number_nodes, max_nodes);
        		        break;
                		}
        		}


		//addRandomEdge(graph, 1);

		addRandomEdgeTest(graph, 1);
		computeClusteringCoefficient(graph);

		//printf("The clustering coefficient of the graph is %lf \n", graph->clustering_coefficient);
		//if the clustering coefficient is less than the specified number then try to improve it.  
		if(graph->clustering_coefficient<CM && graph->number_of_vertices>2) {
			ameliorateClusteringCoefficient(graph, CM);
			}

		pool->graphs[i]=graph;

		}
		//return pool;
		}




/**
 * This function creates a pool that contains randomly generated 3 motifs.
 */
void createPoolof3Motifs(Pool *pool, int size, int precise_number_nodes, double clust_coef) {
	pool->size=size;
	pool->graphs=(Graph**)malloc(size*sizeof(Graph*));

	int i, type, max_nodes, max_regulators;
	int types[]={1,2,3};

	Graph *graph;

	for(i=0;i<size;++i) {

		if(precise_number_nodes>0) {
			type=sample_int(1, 3);
			max_nodes=precise_number_nodes;
		}

		else {
			type=types[i];
			max_nodes=sample_int(MIN_NODES, MAX_NODES);
		}

		//max_nodes=(precise_number_nodes==0)?sample_int(MIN_NODES, MAX_NODES):precise_number_nodes;

		if(max_nodes<3) {
			graph=generateSIM(precise_number_nodes, max_nodes);
		}

		else {
			switch(type) {
				case 1:
					graph=generateSIM(precise_number_nodes, max_nodes);
					break;
				case 2:
					graph=generateDOR(precise_number_nodes, max_nodes);
					break;
				case 3:
					graph=generateFL(precise_number_nodes, max_nodes);
					break;
			}
		}

		//addRandomEdge(graph, 1);
		//addRandomEdgeTest(graph, 1);
		computeClusteringCoefficient(graph);

		//printf("The clustering coefficient of the graph is %lf \n", graph->clustering_coefficient);
		//if the clustering coefficient is less than the specified number then try to improve it.

		if(graph->clustering_coefficient<CM && graph->number_of_vertices>2) {
			//ameliorateClusteringCoefficient(graph, CM);
			ameliorateClusteringCoefficient(graph, clust_coef);
		}

		pool->graphs[i]=graph;
		//if(precise_number_nodes>0) printGraph(pool->graphs[i]);

	}
	//return pool;
}


/**
 * Uniformly randomly chooses a motif from the pool
 */
Graph *sampleMotif(Pool *pool) {
	int size=pool->size;	
	int index=sample_int(1, size);	
	return pool->graphs[index-1];
	}




/**
 * Cleans up the memory
 */
void cleanupPool(Pool *pool) {
	int i;
	for(i=0; i<pool->size; ++i) {
		cleanupGraph(pool->graphs[i]);
	}
	free(pool->graphs);
}


