/**
* Adjacency list for the graph
* Copyright (c) 2015 K.Musayeva <khmusayeva@gmail.com>
**/

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
