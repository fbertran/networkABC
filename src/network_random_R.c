/**
 * This file provides the functionality of construction of a genetic network
 * partially based on the  algorithm by Di Camillo, Barbara, Gianna Toffolo, and Claudio Cobelli. "A gene network simulator to assess reverse engineering algorithms." Annals of the New York Academy of Sciences 1158.1 (2009): 125-142.
 * Copyright (c) 2015 K.Musayeva <khmusayeva@gmail.com>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <time.h>
//for GetRNGstate and PutRNGstate
#include <R.h>
//for Rprintf
#include <R_ext/Print.h>
#include "graph.h"
#include "pool.h"
#include "network.h"
#include "utils.h"
#define SIZE 3 //size of a pool of motifs.



void initV(Network *self) {
        self->V=malloc(self->number_of_nodes*sizeof(int));
	 }



void shuffleV(Network *self) {
	int i;
        for(i=0; i<self->number_of_nodes; ++i) {
                self->V[i]=i;
                }
        shuffle(self->V, self->number_of_nodes);
	 }



void initH(Network *self) {
	self->H=malloc(self->number_of_nodes*sizeof(int));
	}



void initGraph(Network *self) {
        self->G=createGraph(self->number_of_nodes, 0);
	}



void initPool(Network *self) {
	self->pool=malloc(sizeof(Pool));
	}


void updateV(Network *network, int new_size) {
	int i;
	for(i=0;i<new_size;++i) {
		network->V[i]=network->H[i];	
	   }

	}


void resetNetwork(Network *self) {
	resetGraph(self->G);
	shuffleV(self);	
	}



void writeAdjacencyMatrix(Graph *graph, char *output) {
        FILE* fo = fopen(output, "w");
  // Updated to Rprintf to cope with CRAN requirements
   if(fo == NULL) {
                Rprintf("Error in opening the file \n");
                }

        //fprintf(fo,"i j\n");
        int number_of_vertices=graph->number_of_vertices;

        int i,j;
        for(i=0; i<number_of_vertices; ++i) {

                for(j=0; j<number_of_vertices; ++j) {
                          fprintf(fo,"%d ", graph->edges[i][j]);
                        }

                fprintf(fo,"\n");

                }

        fclose(fo);

     }




void writeGraph(Graph *graph, char *output) {

        FILE* fo = fopen(output, "w");

  // Updated to Rprintf to cope with CRAN requirements
  if(fo == NULL) {
		Rprintf("Error in opening the file \n");
		}
   

	
 	fprintf(fo,"i j\n");
	int number_of_vertices=graph->number_of_vertices;

        int i,j;
        for(i=0; i<number_of_vertices; ++i) {
        	for(j=0; j<number_of_vertices; ++j) {

                	if(graph->edges[i][j]!=0) {
			 	fprintf(fo,"%d %d\n", i, j);
				}
                	}
                }

        fclose(fo);

     }



void generate(Network *self, int number_of_nodes, int recursion, double clust_coef) {
	
	if(number_of_nodes==1) {
		return;
		}

	Graph *motif, *mappedMotif;
	int i, k, j, track_regulators=0, q=0, remaining_nodes=number_of_nodes, m=0;
	int counter=0; //loop=0 removed definition due to warning: unused variable 'loop' [-Wunused-variable]

	while(counter<number_of_nodes-1) {
		createPoolof3Motifs(self->pool, SIZE, 0, clust_coef);
		for(i=0; i<SIZE; ++i) {
			motif=self->pool->graphs[i];
			k=motif->number_of_vertices;
			if(k>remaining_nodes) {
				cleanupPool(self->pool);
				createPoolof3Motifs(self->pool, 1, remaining_nodes, clust_coef);
				motif=self->pool->graphs[0];
				k=motif->number_of_vertices;
				}
			mappedMotif=createGraph(k, motif->type);	
			graphcpy(mappedMotif, motif);
			for(j=0; j<k; ++j, ++q) {		
				mappedMotif->vertices[j]->index=self->V[q];
				if(motif->vertices[j]->is_regulator) {
					self->H[track_regulators++]=self->V[q];
					}
				self->V[q]=-1;
				}	

			for(j=0; j<k; ++j) {		
				for(m=0; m<k; m++) {
					self->G->edges[mappedMotif->vertices[j]->index][mappedMotif->vertices[m]->index]=motif->edges[j][m];
					}
				}

			counter+=k;		
			remaining_nodes=number_of_nodes-counter;	
			cleanupGraph(mappedMotif);
			if(remaining_nodes<=1) {
				break;
				}
			}

		cleanupPool(self->pool);

		}

	remaining_nodes=number_of_nodes-counter;	

	if(remaining_nodes>0) {
		for(i=0;i<remaining_nodes;++i, ++q) {
			 self->H[track_regulators++]=self->V[q];	
			 self->V[q]=-1;
			}
		}

	updateV(self, track_regulators);
	generate(self, track_regulators, ++recursion, clust_coef);

	}



void cleanup(Network *self) {
        free(self->V);        
        free(self->H);                
        free(self->pool);             
        cleanupGraph(self->G);
	}



	Network *initNetwork(int number_of_nodes) {
	Network *network=malloc(sizeof(Network));
	network->number_of_nodes=number_of_nodes;
	network->initV=initV;
	network->shuffleV=shuffleV;
	network->initH=initH;
	network->initGraph=initGraph;
	network->initPool=initPool;
	network->generate=generate;
	network->cleanup=cleanup;
	network->resetNetwork=resetNetwork;
	network->initV(network);
	network->shuffleV(network);
	network->initH(network);
	network->initGraph(network);
	network->initPool(network);
	return network;
}


/**
 * To method is to be called from R.
 * n is the number of nodes or genes, array is to store the generated network.
 */
void run(int *n, double *pclust_coef, int *array) {

	int number_of_nodes=*n;
	double clust_coef=*pclust_coef;

	GetRNGstate();
	// replaced by GetRNGstate() and R random generation functions to cope with CRAN requirements
	// srand(time(NULL));
        Network *network=initNetwork(number_of_nodes);
        //printf("Number of nodes: %d\n", network->number_of_nodes);
        network->generate(network, network->number_of_nodes, 0, clust_coef);
	computeDegree(network->G);
        computeClusteringCoefficient(network->G);
        //printf("The clustering coefficient is: %lf\n", network->G->clustering_coefficient);

	int i,j;
	for(i=0;i<number_of_nodes;++i){
		for(j=0;j<number_of_nodes;++j){
			array[j+i*number_of_nodes]=network->G->edges[i][j];
			}
		}

        network->cleanup(network);
       	free(network);

	PutRNGstate();
	}


