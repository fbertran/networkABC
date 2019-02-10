/**
 * This file provides the functionality of construction of a graph.
 * Copyright 2015 K.Musayeva <khmusayeva@gmail.com>
 */

#include "graph.h"


Graph *createGraph(int number_of_nodes, int type) {
        Graph *graph=malloc(sizeof(Graph));
	graph->vertices=malloc(number_of_nodes*sizeof(Vertex*));

	int i;
 	int **edges=malloc(sizeof(int*)*number_of_nodes);

  	Vertex *vertex;	
	for(i=0; i<number_of_nodes; ++i) {	
                vertex=malloc(sizeof(Vertex));
		vertex->degree=0;
		vertex->index=i;
		vertex->connectivity=0;
		vertex->is_regulator=0;
		graph->vertices[i]=vertex;
		edges[i]=calloc(number_of_nodes, sizeof(int));
		}

        graph->number_of_vertices=number_of_nodes;
        graph->edges=edges;
	graph->type=type;
	return(graph);
	 }




void resetGraph(Graph *graph) {

	int i,j;
	for(i=0; i<graph->number_of_vertices; ++i) {
                graph->vertices[i]->degree=0;
                graph->vertices[i]->index=i;
                graph->vertices[i]->connectivity=0;
                graph->vertices[i]->is_regulator=0;
		for(j=0; j<graph->number_of_vertices; ++j) {
                	graph->edges[i][j]=0;
                	}
                }

	}


/**
 * Adds edges between two vertices.
 */
void addEdge(int src, int dest, int is_repressed, Graph *graph) { 
	graph->edges[src][dest]=(is_repressed)?-1:1;
	graph->vertices[src]->degree+=1;	
	graph->vertices[dest]->degree+=1;	
	}



/**
 * Adds a specified number of  edges between two randomly chosen vertices.
 * The edges are added only between operons.
 */
int addRandomEdge(Graph *graph, int number_of_edges) { 
	int number_of_nodes=graph->number_of_vertices;
	int number_of_regulators=graph->number_of_regulators;
  	int number_of_elements=number_of_nodes-number_of_regulators;

	if(number_of_elements<2*number_of_edges) {
		//printf("Number of edges to be added is greater than the number of operons.\n");
		return(1);
	}

	int *array=malloc(number_of_elements*sizeof(int));

	int i,j=0;
	for(i=0; i<number_of_nodes;i++){
		if(graph->vertices[i]->is_regulator==0) {
			array[j++]=i;
			}
		}

	shuffle(array, number_of_elements);	

	int v1, v2, k=0;
	j=0, i=0;

	for(i=0; i<number_of_elements; ++i) {
		v1=array[i];
		for(j=i+1; j<number_of_elements && k<number_of_edges; ++j) {
			v2=array[j];
			if(graph->edges[v1][v2]==0 && graph->edges[v2][v1]==0) {
					addEdge(v1, v2, 0, graph);
					k+=1;
				}
			}
		 }

	free(array);
	return(0);
	}


/**
 * Modification!!!
 * Adds edges between regulators as well.
 */
int addRandomEdgeTest(Graph *graph, int number_of_edges) { 
	int number_of_nodes=graph->number_of_vertices;

	if(number_of_nodes<2*number_of_edges) {
		number_of_edges=((number_of_nodes-1)*number_of_nodes)/2;
		//return(1);
		}

	int *array=malloc(number_of_nodes*sizeof(int));
	int i;
	for(i=0; i<number_of_nodes;i++){
		array[i]=i;
		}
	shuffle(array, number_of_nodes);	

	int v1, v2, k=0, j=0;
	i=0;

	for(i=0; i<number_of_nodes; ++i) {
		v1=array[i];
		for(j=i+1; j<number_of_nodes && k<number_of_edges; ++j) {
			v2=array[j];
		//	printf("%d and %d\n", v1, v2);
			if(graph->edges[v1][v2]==0 && graph->edges[v2][v1]==0) {
				addEdge(v1, v2, 0, graph);
				k+=1;
				}
			}
		 }

	free(array);
	return(0);
	}



/**
 * Adds edges between the neighbours of a given node.
 */
static void addEdgeSelectedNode(int index, int number_of_links, Graph *graph) {
	int i, j, k=0;
	int *array=malloc(graph->number_of_vertices*sizeof(int));
	int number_of_vertices=graph->number_of_vertices;

        for(i=0; i<number_of_vertices; ++i) {
		array[i]=i;
		}
	shuffle(array, number_of_vertices);

	int possible_number_of_links=((number_of_vertices-1)*(number_of_vertices-2))/2;

	//printf("The number of vertices %d\n", number_of_vertices);
	//printf("The possible number of links %d\n", possible_number_of_links);

	if(number_of_links>possible_number_of_links) {
		number_of_links=possible_number_of_links;
		}	

	//printf("The number of links to be added %d\n", number_of_links);

	/*
        for(i=0; i<graph->number_of_vertices; ++i) {
		if(index==i) {continue;}
                for(j=0; j<graph->number_of_vertices && k<links; ++j) {
			if(i==j) {continue;}
			if((graph->edges[index][i]!=0 || graph->edges[i][index]!=0) && (graph->edges[index][j]!=0 || graph->edges[j][index]!=0) && (graph->edges[i][j]==0 && graph->edges[j][i]==0)) {
				addEdge(i, j, 0, graph);
				k+=1;
				}
			}
		}
		*/

	int a,b;
        for(i=0; i<number_of_links;++i) {

		a=sample_int(0, number_of_vertices-1);
		b=sample_int(0, number_of_vertices-1);	

		//printf("a = %d\n", a);
		//printf("b = %d\n", b);

		while(b==a) {
			b=sample_int(0, number_of_vertices-1);	
			}

		if((graph->edges[index][a]!=0 || graph->edges[a][index]!=0) && (graph->edges[index][b]!=0 || graph->edges[b][index]!=0) && (graph->edges[a][b]==0 && graph->edges[b][a]==0)) {
			addEdge(a, b, 0, graph);
			}
		}

		free(array);

	}



/**
 * Adds extra links between the nodes to increase the clustering coefficient.
 */
void ameliorateClusteringCoefficient(Graph *graph, double CM) {
	int i, random_connectivity, desired_number_links;
	double mean, std=1.0, tmp;
	Vertex *vertex;
	
	for(i=0;i<graph->number_of_vertices;++i) {

		vertex=graph->vertices[i];

		mean=((double)CM*(vertex->degree)*(vertex->degree-1))/2.0;

		sampleFromGaussian(mean, std, &tmp);	

		if(tmp<=0) {continue;}

		random_connectivity=tmp+0.5;		

		desired_number_links=random_connectivity-vertex->connectivity;

		if(desired_number_links>0) {
			addEdgeSelectedNode(i, desired_number_links, graph);								
			computeClusteringCoefficient(graph);
			if(graph->clustering_coefficient>=CM) {break;}
			}

		}	

	}



/**
 * Computes the clustering coefficient of a graph.
 */
double computeClusteringCoefficient(Graph *graph) {
	int i, j, m, K;
	int n=graph->number_of_vertices;
	double vertex_C, C, sum=0.0;
	Vertex *vertex;

	for(i=0;i<n;++i) {
		vertex=graph->vertices[i];
		K=0;
		for(j=0;j<n;++j) {
			if(i==j) continue;
			if(graph->edges[i][j]!=0 || graph->edges[j][i]!=0) {
				for(m=j+1;m<n;++m) {
					if(m==i) continue;
					if((graph->edges[m][i]!=0 || graph->edges[i][m]!=0) && (graph->edges[j][m]!=0 || graph->edges[m][j]!=0)) {
						K+=1;
						}		
					}
				}
			}		

		//printf("Vertex %d, K is %d , vertex degree is %d \n", i, K, vertex->degree);
		vertex_C=(vertex->degree==1)?0:(double)(2*K)/(double)(vertex->degree*(vertex->degree-1));
		vertex->connectivity=K;
		sum+=vertex_C;
		//printf("The clustering coefficient of vertex %d is %lf \n", i, vertex_C);
		}

	C=sum/((double)n);
	graph->clustering_coefficient=C;
	return C;
	}



/**
 * Computes the degree distribution of a (connected) graph
 */
/*
int *getDegreeDistribution(Graph *graph) {

	int *array=(int*)calloc(graph->number_of_vertices, sizeof(int));
	Vertex *vertex;

	int i;
	for(i=0;i<graph->number_of_vertices;++i) {
		vertex=graph->vertices[i];		
		//printf("%d ", vertex->degree);
		array[vertex->degree-1]+=1;
		}	

	//printf("\n");
	return array;	
	}
*/



/**
 * Computes the degree distribution of a graph
 */
void getDegreeDistribution(Graph *graph, double *F) {
	Vertex *vertex;
	int i;

	for(i=0;i<graph->number_of_vertices;++i) {
		F[i]=0;
		}	

	for(i=0;i<graph->number_of_vertices;++i) {
		vertex=graph->vertices[i];		
		if(vertex->degree==0) {continue;}
		F[vertex->degree-1]+=1.0;
		}	


	for(i=0;i<graph->number_of_vertices;++i) {
		F[i]/=graph->number_of_vertices;
		}

	}



/**
 * Computes the degree of each vertex of the graph
 */
void computeDegree(Graph *graph) {
	int n=graph->number_of_vertices;
	int i, j;

	for(j=0;j<n;++j) {
		graph->vertices[j]->degree=0;
		}	
	
	for(i=0;i<n;++i) {
		for(j=0;j<n;++j) {
			if(graph->edges[i][j]!=0 || graph->edges[j][i]!=0) {
				graph->vertices[i]->degree+=1;
				}

			}
		}
	}




/**
 * Prints out the graph structure
 */
void printGraph(Graph *graph) {
        Vertex *vertex;
        int i,j,index;

	//printf("Type of the graph %d \n", graph->type);
	//printf("Number of regulators %d \n", graph->number_of_regulators);

	//computeClusteringCoefficient(graph);
	//printf("The average clustering coefficient is: %lf \n", graph->clustering_coefficient);


	/*
        for(i=0; i<graph->number_of_vertices; ++i) {
		if(graph->vertices[i]->is_regulator) {
			printf("Regulator: %d\n", graph->vertices[i]->index);
			}
		}
	*/	

	//printf("Number of vertices %d \n", graph->number_of_vertices);

        for(i=0; i<graph->number_of_vertices; ++i) {
		for(j=0; j<graph->number_of_vertices; ++j) {	
			printf("%d ", graph->edges[i][j]);
                	}
		printf("\n");
        	}


	/*
	printf("\nDegree distribution of the graph\n");
	int *array=getDegreeDistribution(graph);
	double n=(double)graph->number_of_vertices;
        for(i=0; i<graph->number_of_vertices; ++i) {
		printf("%lf ", (double)array[i]/n);
		}
	printf("\n...............................\n");
	free(array);
	*/


	}



/**
 * Copies the graph structure
 */
void graphcpy(Graph *dest, Graph *src) {
	dest->type=src->type;
	dest->number_of_vertices=src->number_of_vertices;
	dest->number_of_regulators=src->number_of_regulators;
	dest->clustering_coefficient=src->clustering_coefficient;

	int i, j;
	for(i=0; i<src->number_of_vertices; ++i) {
		dest->vertices[i]->is_regulator=src->vertices[i]->is_regulator;
        	dest->vertices[i]->connectivity=src->vertices[i]->connectivity;
	        dest->vertices[i]->degree=src->vertices[i]->degree;

		for(j=0; j<src->number_of_vertices; ++j) {
			dest->edges[i][j]=src->edges[i][j];
			}

		}


	}



/**
 *
 */
void restructureGraph(Graph *graph, int *nodes, int *labels, int* array1, int* array2) {
  int row, col;
  int index_x, index_y, source, dest;


  //permute rows
  for(row=0;row<graph->number_of_vertices;row++) {
         graph->vertices[nodes[row]]->index=labels[row];
         index_x=labels[row];
         for(col=0;col<graph->number_of_vertices;col++) {
                         array1[col+graph->number_of_vertices*index_x]=graph->edges[nodes[row]][col]; 
                         array2[col+graph->number_of_vertices*index_x]=graph->edges[nodes[row]][col]; 
                        }
                }


  //permute columns
  for(col=0;col<graph->number_of_vertices;col++) {
                //index_x=graph->vertices[nodes[col]]->index;
                index_x=labels[col];
                for(row=0;row<graph->number_of_vertices;row++) {
                        array1[index_x+graph->number_of_vertices*row]=array2[nodes[col]+graph->number_of_vertices*row];//permute columns
                                }
                        }


  //restruture the original network
  for(col=0;col<graph->number_of_vertices;col++) {
                for(row=0;row<graph->number_of_vertices;row++) {
			graph->edges[col][row]=array1[row+col*graph->number_of_vertices];
                                }
                        }


	}


/**
 *
 */
void restructureGraph1(Graph *graph, int *nodes, int *labels, int* array1, int* array2, int *config) {
  int row, col;
  int index_x, index_y, source, dest;

	  for(row=0;row<graph->number_of_vertices;row++) {
		 graph->vertices[nodes[row]]->index=labels[row];
		 index_x=labels[row];
		 for(col=0;col<graph->number_of_vertices;col++) {
				 array1[col+graph->number_of_vertices*index_x]=graph->edges[nodes[row]][col];
				 array2[col+graph->number_of_vertices*index_x]=graph->edges[nodes[row]][col];
				}
			}


	  for(col=0;col<graph->number_of_vertices;col++) {
			index_x=labels[col];
			for(row=0;row<graph->number_of_vertices;row++) {
				 array1[index_x+graph->number_of_vertices*row]=array2[nodes[col]+graph->number_of_vertices*row];//permute columns
					}
				 }
		       

	  for(col=0;col<graph->number_of_vertices;col++) {
			for(row=0;row<graph->number_of_vertices;row++) {
				graph->edges[col][row]=array1[row+col*graph->number_of_vertices];
					}
				}

	if(graph->edges[labels[0]][labels[1]]!=config[0]&&graph->edges[labels[1]][labels[0]]!=config[0]) {
	 	graph->edges[labels[0]][labels[1]]=config[0];
		}

	if(graph->edges[labels[0]][labels[2]]!=config[1]&&graph->edges[labels[2]][labels[0]]!=config[1]) {
	 	graph->edges[labels[0]][labels[2]]=config[1];
		}

	if(graph->edges[labels[2]][labels[1]]!=config[2]&&graph->edges[labels[1]][labels[2]]!=config[2]) {
	 	graph->edges[labels[1]][labels[2]]=config[2];
		}
	 

        }



/**
 * Frees up the memory used by the graph
 */
void cleanupGraph(Graph *graph) {
	int i;

        for(i=0; i<graph->number_of_vertices; ++i) {
		free(graph->edges[i]);
		free(graph->vertices[i]);
                  }

	free(graph->vertices);
	free(graph->edges);
	free(graph);

        }


