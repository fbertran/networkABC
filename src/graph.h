/**
* Graph data structure. 
 * 2015. Author K.Musayeva <khmusayeva@gmail.com>
 * 2019. Fixes and updates by F. Bertrand <fbertran@math.unistra.fr>
 * Copyright (c) Universite de Strasbourg and CNRS
 */

#ifndef GRAPH_H
#define GRAPH_H

typedef struct vertex_str {
	int index;
	int degree;
	int is_regulator;
	int connectivity;
} Vertex;


typedef struct graph_str {
	int type;
	int number_of_vertices;
	int number_of_regulators;
	int **edges;	
	double clustering_coefficient;
	Vertex **vertices;
} Graph;


Graph *createGraph(int, int);
void resetGraph(Graph *graph);
void addEdge(int, int, int, Graph*);
int addRandomEdge(Graph*, int);
double computeClusteringCoefficient(Graph*);
void printGraph(Graph*);
void restructureGraph(Graph*, int*, int*, int*, int*);
void computeDegree(Graph *graph);
void ameliorateClusteringCoefficient(Graph *graph, double CM);
int addRandomEdgeTest(Graph *graph, int number_of_edges);
void graphcpy(Graph *dest, Graph *src);
void cleanupGraph(Graph *graph);

#endif

