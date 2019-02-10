/**
* Graph data structure. 
* Copyright (c) 2015 K.Musayeva <khmusayeva@gmail.com>.
**/

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
void addEdge(int, int, int, Graph*);
int addRandomEdge(Graph*, int);
double computeClusteringCoefficient(Graph*);
void printGraph(Graph*);
void restructureGraph(Graph*, int*, int*, int*, int*);

#endif

