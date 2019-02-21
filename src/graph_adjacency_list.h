/**
* Adjacency list for the graph
 * 2015. Author K.Musayeva <khmusayeva@gmail.com>
 * 2019. Fixes and updates by F. Bertrand <fbertran@math.unistra.fr>
 * Copyright (c) Universite de Strasbourg and CNRS
 */

typedef struct vertex_str {
	int index;
	int degree;
	struct vertex_str *r_next; //the node it regulates
	struct vertex_str *i_next; //the node it represses (inhibits) 
} Vertex;

typedef struct graph_str {
	int type;
	int number_of_vertices;
	Vertex **vertices;	
} Graph;

Graph *createGraph(int, int);
void addEdge(int, int, int, Graph*);
void deleteNeighbour(int, int, Graph);
void printGraph(Graph*);
