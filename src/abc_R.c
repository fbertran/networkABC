/**
 * This program realizes the ABC algorithm.
 * 2015. Author K.Musayeva <khmusayeva@gmail.com>
 * 2019. Fixes and updates by F. Bertrand <frederic.bertrand@lecnam.net>
 * Copyright (c) Universite de Strasbourg and CNRS
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alloc2d.h"
#include <math.h>
//#include <unistd.h>
#include <time.h>
#include <R.h> /* for GetRNGstate and PutRNGstate */
#include <R_ext/Utils.h>  /* R_CheckUserInterrupt */
#include <R_ext/Print.h>
#include "graph.h"
#include "pool.h"
//#include "network.h"
#include "network_random_R.h"
#include "utils.h"
#define SQUARE(x) (x*x)
//for unif_rand
#include <Rmath.h>


/**
* Reads gene expression into a two-dimensional array 
*/
int read_data(double *array, int number_genes, int number_times, double ***data) {
	//double entry; removed definition due to warning: unused variable 'entry' [-Wunused-variable]

        int i, j;

	//printf("Number of time points: %d \n", *number_times);
        *data = (double**) malloc(number_genes * sizeof(double*));
        for(i=0; i<number_genes; ++i) {
		(*data)[i] = (double*) malloc(number_times*sizeof(double));
		}
	
        for(i=0; i<number_genes; i++) {
         for (j=0; j<number_times; j++) {
		(*data)[i][j]=array[j+i*number_times];
		//printf("%lf ", (*data)[i][j]);
                     }
		//printf("\n");
                }

	return(0);
        }




/**
 * For debugging purposes: writes the distance array into a file
 **/
void writeArray(double *dist, int size, char *output) {

        FILE* fo = fopen(output, "w");

  // Updated to Rprintf to cope with CRAN requirements
  if(fo == NULL) {
                Rprintf("Error in opening the file \n");
                }

	int i;
        for(i=0; i<size; ++i) {
                fprintf(fo,"%lf\n", dist[i]);
                }

        fclose(fo);

     }




/**
 * For debugging purposes: writes the simulated data into a file
 */
void writeSimulatedData(double **data, int number_times, int number_genes, char *output) {

        FILE* fo = fopen(output, "w");

  // Updated to Rprintf to cope with CRAN requirements
  if(fo == NULL) {
                Rprintf("Error in opening the file \n");
                }
   


        int i,j;
	for(i=0; i<number_genes; ++i) {
		for(j=0; j<number_times; ++j) {
			fprintf(fo,"%lf ", data[i][j]);
                        }
		fprintf(fo,"\n");
                } 
                                                                                                                                
	fclose(fo);
                                                                                                                                       
   }




/**
 * For debugging purposes: writes the omega matrix into a file
 */
void writeOmega(int number_genes, double *omega, char *output) {
        FILE* fo = fopen(output, "w");

  // Updated to Rprintf to cope with CRAN requirements
  if(fo == NULL) {
                Rprintf("Error in opening the file \n");
                }
  


	int i,j;

	for(i=0; i<number_genes; i++) {
		for (j=0; j<number_genes; j++) {
			fprintf(fo,"%lf ", omega[j+i*number_genes]);
                     	 }
		fprintf(fo,"\n");
   		 }
	fclose(fo);

	}



/**
* Generates a random omega matrix
*/
void generateOmega(double *omega, int number_genes, int min, int max) {
	int row, col;

	for(row=0;row<number_genes;row++) {
		for(col=0;col<number_genes;col++) {
			omega[col+number_genes*row]=generateRandomNumber(min, max);
			}
		}

	}




/**
 * Generates  an omega matrix from the given network
 */
void generateOmegaFromNetwork(Graph *G, double *omega, int number_genes, int min, int max) {
        int row, col;
		//int index_x, index_y, source, dest; removed definition due to warning: unused variable [-Wunused-variable]

        for(row=0;row<number_genes;row++) {
                for(col=0;col<number_genes;col++) {
                        if(G->edges[row][col]!=0) {
                                omega[col+number_genes*row]=1;
                                }
                        }
                }

        }


/**
* Reads the omega matrix from the file
*/
int readOmega(char *input, double *omega, int number_genes) {

  double entry;
  int row, col;

  if (number_genes <= 0) {
    Rf_error("readOmega: number_genes must be > 0 (got %d).", number_genes);
  }
  
	FILE* fi = fopen(input, "r");
	if(fi == NULL) {
	  // Updated to Rprintf to cope with CRAN requirements
	  Rprintf("Cannot open the file: %s\n", input);
		return(1);
             }

        for(row=0;row<number_genes;row++) {
           for(col=0;col<number_genes;col++) {
             /* leading space in format skips any whitespace/newlines */
             int rc = fscanf(fi, " %lf", &entry);
             if (rc != 1) {
               fclose(fi);
               Rf_error("readOmega: failed to read omega[%d,%d] as double (rc=%d).", row, col, rc);
               }
             omega[(size_t)col+(size_t)number_genes*(size_t)row]=entry;
             }
           }

        fclose(fi);
	return(0);
	}


/**
 * Resets the entries of the omega matrix to zero.
 * array1 and array2 are also initialized, which are used in the restructuring of the network.
 */
void resetOmega(double *omega, int *array1, int *array2, int number_genes) {
	int i,j;
	for(i=0; i<number_genes; i++) {
		for (j=0; j<number_genes; j++) {
               		omega[j+i*number_genes]=0;
               		array1[j+i*number_genes]=0;
               		array2[j+i*number_genes]=0;
               		 }
        	 }	
	}




/**
 * Allocates simulated data and copies the first time step from the observed data into it
 */
void initSimulatedData(double **data, double ***simulated_data, int number_genes, int number_times) {
	int i;
        for(i=0; i<number_genes; ++i) {
           (*simulated_data)[i] = (double*)calloc(number_times,sizeof(double));
       	   (*simulated_data)[i][0] = data[i][0]; 
            }

	}


/**
 * Resets the simulated data to zero
 */
void resetSimulatedData(double **simulated_data, int number_genes, int number_times) {
	int i,j;
	
        for(i=0; i<number_genes; ++i) {
        	for(j=1; j<number_times; ++j) {
       	  		 simulated_data[i][j]=0; 
        	    	}
		    }

  	}



/**
* Simulates data based on omega matrix
*/
void simulateDataOneStepBack(double **data, double **simulated_data, int number_genes, int number_times, double *omega) {
	int i, j, t;
	double result;
	for(t=1; t<number_times; t++) {
		for(i=0; i<number_genes; i++) {
			result=0.0;
			for(j=0; j<number_genes; j++) {
					result+=omega[i+j*number_genes]*simulated_data[j][t-1];
					}
			simulated_data[i][t]=result;
				}
			}
	}



/**
 * Generates micro-array data given the first time step
 * */
void generateData(char *input, 
                  int number_genes, 
                  int number_times, 
                  double ***data, 
                  double **data_block_out,
                  double *omega, 
                  double *F) {

  if (number_genes <= 0 || number_times <= 0) {
    Rf_error("generateData: number_genes and number_times must be > 0 (got %d, %d).",
             number_genes, number_times);
  }
  //char buf[2000]; removed definition due to warning: unused variable 'buf' [-Wunused-variable]`
  
  FILE* fi = fopen(input, "r");
  if (fi == NULL) {
    // Updated to Rprintf to cope with CRAN requirements
    Rf_error("generateData: cannot open file '%s': %s", input, strerror(errno));
    }
  
  double **M; double *M_block = NULL;
  if (alloc2d_contig(&M, &M_block, number_genes, number_times) != 0) {
    fclose(fi);
    Rf_error("generateData: out of memory.");
  }
  *data = M;
  if (data_block_out) *data_block_out = M_block;
  
  /* lecture t = 0 (a double per gene) */
  for (int i = 0; i < number_genes; ++i) {
    double entry; int rc = fscanf(fi, " %lf", &entry);
    if (rc != 1) {
      fclose(fi);
      free2d_contig(M, M_block);
      Rf_error("generateData: failed to read data[%d,0] (rc=%d).", i, rc);
    }
    M[i][0] = entry;
  }
  fclose(fi);
  
  /* propagation t >= 1 ; Omega[i,j] stockée en colonne-major: i + n*j */
  for (int t = 1; t < number_times; ++t) {
    if ((t & 0x3F) == 0) R_CheckUserInterrupt();
    for (int i = 0; i < number_genes; ++i) {
      double acc = 0.0;
      for (int j = 0; j < number_genes; ++j) {
        acc += omega[(size_t)i + (size_t)number_genes * (size_t)j] * M[j][t-1];
      }
      M[i][t] = acc;
    }
  }
}  



/**
* Calculates the Euclidean distance between the simulated and observed data
*/
double computeAR(double **data, double **simulated_data, int number_genes, int number_times) {
	int i, j;
	double result=0, temp;

	for(i=0;i<number_genes;i++){
		for(j=0;j<number_times;j++) {
			temp=data[i][j]-simulated_data[i][j];
			result+=SQUARE(temp);
			}
		}

	return sqrt(result);
	}




/**
 * Calculates the Euclidean distance for each gene of the simulated and observed data 
 */
void computeARForEachGene(double **data, double **simulated_data, int number_genes, int number_times, double *dist) {
        int i, j;
        double result, temp;

        for(i=0;i<number_genes;i++) {
		result=0;
                for(j=0;j<number_times;j++) {
                        temp=data[i][j]-simulated_data[i][j];
                        result+=SQUARE(temp);
                        }
		dist[i]=sqrt(result);		
                }

	}



/**
 * Computes hub probabilities 
 */
void compute_hub_probs(double *probs, double *cumul_probs, int *hubs, int number_genes) {

        int i, sum=0;

        for(i=0; i<number_genes; ++i) {
                sum+=hubs[i];
                }

	//printf("Probs:");
        for(i=0; i<number_genes; ++i) {
                probs[i]=(double)hubs[i]/sum;
		hubs[i]=0;
	//	printf("%lf ", probs[i]);
                }

        //printf("\n");

	}




/**
 * Provides the functionality of labeling hubs according to their probabilites of being a hub.
 */
void label_hubs(int *labels, double *probs, double *cumul_probs, int number_genes, int number_hubs) {
        double random_number, max;
        int i, j, k, elm; 	//, t, z=0 removed definition due to warning: unused variable 't', 'z' [-Wunused-variable]
	for(i=0; i<number_genes; i++) {
		labels[i]=i;
		}
	for(i=1; i<=number_genes; ++i) {
                cumul_probs[i]=cumul_probs[i-1]+probs[i-1];
                }
        max=cumul_probs[number_genes];
	int temp, index=0;
        for(i=1; i<=number_hubs; ++i){
				random_number=unif_rand() * max;
				// Replaced by R random generator; required by CRAN
                // random_number=((double)rand()/RAND_MAX) * max;
                for(j=1; j<=number_genes; j++){
                        if(random_number<=cumul_probs[j] && random_number>cumul_probs[j-1]) {
                                        elm=j-1;
                                        max=cumul_probs[number_genes]-probs[elm];
                                        cumul_probs[j]=cumul_probs[j-1];
                                        for(k=j+1; k<=number_genes; k++){
                                                cumul_probs[k]-=probs[elm];
                                                }
					temp=labels[index];
					labels[index]=labels[elm];
					labels[elm]=temp;
					index++;
                                        break;
               	                }
			 }
		 }
	
	}




/*
*Computes frequencies of being a neighbour
*/
void compute_neighbour_freq(Graph *graph, int *neighbour_freqs, int number_genes) {
	
	int i, j;

       	for(i=0; i<number_genes; ++i) {
       		for(j=0; j<number_genes; ++j) {
			if(graph->edges[i][j]!=0) {
				neighbour_freqs[j+i*number_genes]+=1;
				}
			}
       		}
		
		/*
		printf("====================\n");
   		for(i=0; i<number_genes; ++i) {
       			for(j=0; j<number_genes; ++j) {
				printf("%d ", neighbour_freqs[j+i*number_genes]);
				}
			printf("\n");
			}
		printf("====================\n");
		*/
		
	}



/**
*Computes the relative frequencies of being neighbours
*/
void compute_neighbour_probs(double *neighbour_probs, int *neighbour_freqs, int number_genes, int hits) {

        int i, j;
		//double sum; removed definition due to warning: unused variable 'sum' [-Wunused-variable]

        for(i=0; i<number_genes; ++i) {
                for(j=0; j<number_genes; ++j) {
                         neighbour_probs[j+i*number_genes]=(double)neighbour_freqs[j+i*number_genes]/hits;
                         neighbour_freqs[j+i*number_genes]=0;
                        }
                }


        }


/**
 * Turns the clustering coefficient frequencies into the probabilities
 */
void compute_clust_probs(double *prob, int *freq, int size){
        int i;
        double sum=0.0;

        for(i=0; i<size; ++i) {
                sum+=freq[i];
                }

        for(i=0; i<size; ++i) {
                prob[i]=(double)freq[i]/sum;
                freq[i]=0;
                }


        }



/**
 *Samples a configuration and rearranges the labels for hubs according to the sample configuration
 */
void rearrange_labels_for_hubs(int size, double *config_probs, int **configs, int *labels, int number_hubs) {
	int i;

	int config=sample(size, config_probs);

	for(i=0; i<number_hubs; ++i) {
	       labels[i]=configs[config][i];
             }
        

	}



/**
 * Labels non-hubs
 **/
void label_non_hubs(Graph *graph, double *neighbour_probs, int *labels, int *nodes, int labeled_size, int recursion){
	//printf("Recursion:%d\n", recursion);
	if(labeled_size+1==graph->number_of_vertices) {
		return;
		}
	
	int i, j, position;		//num_connections, removed definition due to warning: unused variable 'num_connections' [-Wunused-variable]
	double fraction, temp=0.0;
	int unlabeled_size=graph->number_of_vertices-labeled_size;


	//finds a template position to be labelled
	for(i=labeled_size; i<graph->number_of_vertices; ++i) {
               	int num_connections=0; 

		for(j=0; j<labeled_size; ++j) { 
			if(graph->edges[nodes[i]][nodes[j]]!=0 || graph->edges[nodes[j]][nodes[i]]!=0) {
				num_connections+=1;
				}
			}

		fraction=(double)num_connections/(double)graph->vertices[nodes[i]]->degree;
        	
		if(fraction-temp>0.0000001) {
			position=i;
			temp=fraction;
                        }

		}
	

	double *probs=(double*)malloc(unlabeled_size*sizeof(double));
	double result;

	//for each label compute the probability of being a neighbour to hubs
	for(i=labeled_size; i<graph->number_of_vertices; ++i) {
		result=1.0;
		for(j=0; j<labeled_size; ++j) {
			//check if the position has a link to a given hub, if yes compute the neighbouring probability to it
			if(graph->edges[nodes[position]][nodes[j]]!=0 || graph->edges[nodes[j]][nodes[position]]!=0) {
				//printf("Node %d and %d connected\n", nodes[position], nodes[j]);
				result*=neighbour_probs[labels[i]*graph->number_of_vertices+labels[j]];					
				}

			}
		
		//save the result for a given label in the probabilities array
		probs[i-labeled_size]=result;	
		}

	//samples the label for that position according to probs
	int index=sample(unlabeled_size, probs);	
	index+=labeled_size;
		
	//moves the chosen label ahead in the array to synchronize with the nodes
	swap(labels, labeled_size, index);
	swap(nodes, labeled_size, position);

	free(probs);
	label_non_hubs(graph, neighbour_probs, labels, nodes, ++labeled_size, ++recursion);
	}




/**
 * The ABC algorithm
 */
void C_abc(double *data_array, int *pnumber_genes, int *pnumber_times, int *pclust_size, double *clust_coeffs,
		 double *ptolerance, int *pnumber_hubs, int *pnumber_outer_iter, int *pnumber_inner_iter, int *number_accepted,
		 double *dist_array, double *hub_probs, double *rneighbour_probs, int *is_probs) {

	//srand(time(NULL));
	GetRNGstate();

	double tolerance=*ptolerance;
	//printf("Tolerance: %lf\n", tolerance);
	int number_hubs=*pnumber_hubs;
	int number_outer_iter=*pnumber_outer_iter;
	int number_inner_iter=*pnumber_inner_iter;
	int number_genes=*pnumber_genes;
	int number_times=*pnumber_times;
	//double clust_coef; removed definition due to warning: unused variable 'clust_coef' [-Wunused-variable]


	//printf("number_genes=%d, number_times=%d, tolerance=%lf, number_of_hubs=%d, number_outer_iteration=%d, number_inner_iteration=%d, is_probs=%d, clust_size=%d\n", number_genes, number_times, tolerance, number_hubs, number_outer_iter, number_inner_iter, *is_probs, *pclust_size);

	double **data;

	read_data(data_array, number_genes, number_times, &data);

	int *hubs=calloc(number_genes, sizeof(int)); //for computing the frequency of each node of being in the role of a hub
	int *nodes=malloc(number_genes*sizeof(int)); //for sorting the nodes of a network according to their connectivity
	int *degrees=malloc(number_genes*sizeof(int)); //for sorting the degrees of the nodes of a network
	int *labels=malloc(number_genes*sizeof(int)); //labels computed according to the probabilities of each node being a hub

	double *probs=calloc(number_genes, sizeof(double)); //probability of hubs
	double *cumul_probs=calloc((number_genes+1),sizeof(double)); //cumulative probability of hubs
	int *track_labels=malloc(number_genes*sizeof(int));//an auxillary array

	double *omega = (double*) calloc(number_genes * number_genes, sizeof(double)); //the matrix Omega
	double **simulated_data = (double**) malloc (number_genes * sizeof(double*)); //holds the simulated data
	int *neighbour_freqs=(int*)calloc(number_genes*number_genes, sizeof(int));
	double *neighbour_probs=(double*)calloc(number_genes*number_genes, sizeof(double));

	int *array1 = (int*) calloc(number_genes * number_genes, sizeof(int)); //for the restructuring of the network
	int *array2 = (int*) calloc(number_genes * number_genes, sizeof(int)); //for the restructuring of the network

	int i, j, l, p, q;	//, t, m=0; removed definition due to warning: unused variable 't', 'm' [-Wunused-variable]

	int c=number_hubs*(number_hubs-1)/2;
	int *place_holder=malloc(c*sizeof(int));
	int number_perms=factorial(number_hubs);
	double *config_probs=malloc(number_perms*sizeof(double));
	int **configs=malloc(number_perms*sizeof(int*));

	double *prob_clust_coeffs=calloc(*pclust_size, sizeof(double));
	int *freq_clust_coeffs=calloc(*pclust_size, sizeof(int));


	for(i=0; i<number_perms; ++i) {
		configs[i]=malloc(number_hubs*sizeof(int));
	}


	for(i=0;i<number_genes;++i) {
		labels[i]=i;
	}

	initSimulatedData(data, &simulated_data, number_genes, number_times);
	Network *network=initNetwork(number_genes);

	int hits=0;
	int dist_track=0;

	//check if user has specified the probabilities for hubs and neighbours, if yes use them

	if(*is_probs) {
		//printf("Probabilities are set...\n");
		for(p=0; p<number_genes; ++p) {
			probs[p]=hub_probs[p];
			for(q=0; q<number_genes; ++q) {
				//printf("%lf ", rneighbour_probs[q+p*number_genes]);
				neighbour_probs[q+p*number_genes]=rneighbour_probs[q+p*number_genes];
			}
			//printf("\n");
		}
	}

	/*
    for(p=0; p<number_genes; ++p) {
        printf("%lf ", probs[p]);
        }
    printf("\n====================================\n");
    for(p=0; p<number_genes; ++p) {
        for(q=0; q<number_genes; ++q) {
            printf("%lf ", neighbour_probs[q+p*number_genes]);
            }
        printf("\n");
        }
    */


	for(j=0; j<number_outer_iter; ++j) {
		hits=0;
	  // Updated to Rprintf to cope with CRAN requirements
	    Rprintf("===============================\n");
      Rprintf("Iteration=%d\n", j+1);
		//printf("Tolerance=%lf\n", tolerance);

		for(i=0; i<number_inner_iter; ++i) {

			int clust_index=sample(*pclust_size, prob_clust_coeffs);
			double clust_coef=clust_coeffs[clust_index];
			network->generate(network, number_genes, 0, clust_coef);
			computeDegree(network->G);
			computeClusteringCoefficient(network->G);
			//printGraph(network->G);
			//sorts the nodes for labeling
			for(q=0;q<number_genes;q++) {
				nodes[q]=network->G->vertices[q]->index;
				degrees[nodes[q]]=network->G->vertices[q]->degree;
			}

			//sorts the nodes of the network in the descending order according to their degrees
			sort(degrees, nodes, number_genes);

			//starts labeling
			if(j>0 || *is_probs) {

				/*
				printf("Probabilities of being a neighbour: \n");
   				for(p=0; p<number_genes; ++p) {
       					for(q=0; q<number_genes; ++q) {
						printf("%lf ", neighbour_probs[q+p*number_genes]);
						}
					printf("\n");
					}
				*/

				label_hubs(labels, probs, cumul_probs, number_genes, number_hubs);//get the labeling for the network

				/*
				printf("After labeling the hubs:\n");
				for(p=0; p<number_genes; ++p) {
					printf("%d ", labels[p]);
					}
				printf("\n");
				*/

				//labelAllNodes(labels, track_labels, probs, cumul_probs, number_genes);//get the labeling for the network
				double result=1.0;

				//If there are 3 hubs (labeled as 0, 1 and 2), to indicate whether they are neighbours in the generated network uses 01, 02, and 12 coding.
				//01 considers the nodes 0 and 1, if they are neighbours the corresponding entry will be 1, otherwise it will be 0.
				int k=0;
				for(p=0; p<number_hubs; ++p) {
					for(q=p+1; q<number_hubs; ++q) {
						place_holder[k++]=network->G->edges[nodes[p]][nodes[q]];
					}
				}


				/*
				for(l=0; l<3; ++l) {
					printf("%d ", place_holder[l]);
						}
					printf("\n");
				*/


				permute(labels, 0, number_hubs, configs, number_perms);


				/*
				for(p=0; p<number_perms; ++p) {
					for(q=0; q<number_hubs; ++q) {
						printf("%d ", configs[p][q]);
						}
					printf("\n");
					}
				*/

				for(l=0; l<number_perms; ++l){
					result=1.0;
					k=0;
					for(p=0; p<number_hubs; ++p) {
						for(q=p+1; q<number_hubs; ++q) {
							if(place_holder[k]!=0){
								result*=neighbour_probs[configs[l][p]*number_genes+configs[l][q]];
							}
							else {
								result*=1-neighbour_probs[configs[l][p]*number_genes+configs[l][q]];
							}
							//printf("%lf \n", result);
							++k;
						}
					}
					config_probs[l]=result;
				}


				/*
				for(l=0; l<number_perms; ++l){
					printf("%lf ", config_probs[l]);
					}

				printf(" \n");
				*/

				rearrange_labels_for_hubs(number_perms, config_probs, configs, labels, number_hubs);
				label_non_hubs(network->G, neighbour_probs, labels, nodes, number_hubs, 0);
				restructureGraph(network->G, nodes, labels, array1, array2);

			}

			generateOmegaFromNetwork(network->G, omega, number_genes, -1, 1);

			//Simulates the data
			simulateDataOneStepBack(data, simulated_data, number_genes, number_times, omega);

			//print the distance between the observed and simulated data
			double dist=computeAR(data, simulated_data, number_genes, number_times);

			if(j==number_outer_iter-1) {
				dist_array[dist_track++]=dist;
			}

			if(dist<tolerance) {
				//printf("distance:%lf\n", dist);
				//printf("=====================\n");
				//printGraph(network->G);
				//printf("=====================\n");
				for(q=0;q<number_hubs;q++) {
					hubs[network->G->vertices[nodes[q]]->index]+=1;
				}
				compute_neighbour_freq(network->G, neighbour_freqs, number_genes);
				freq_clust_coeffs[clust_index]+=1;
				++hits;
			}

			network->resetNetwork(network);
			resetOmega(omega, array1, array2, number_genes);
			//resetSimulatedData(simulated_data, number_genes, number_times);
		}

		//printf("Tolerance:%lf\n", tolerance);
		// Updated to Rprintf to cope with CRAN requirements
		  Rprintf("Accepted:%d\n", hits);
		if(hits<10) {
		  // Updated to Rprintf to cope with CRAN requirements
  		  Rprintf("\nAccepted number of nodes is too small or zero.\n");
			break;
		}

		/*
		for(q=0; q<number_genes; ++q) {
			printf("%d\n", hubs[q]);
			}
		*/

		compute_hub_probs(probs, cumul_probs, hubs, number_genes);

		/*
		for(q=0; q<number_genes; ++q) {
			printf("%lf\n", probs[q]);
			}
		*/


		compute_neighbour_probs(neighbour_probs, neighbour_freqs, number_genes, hits);
		compute_clust_probs(prob_clust_coeffs, freq_clust_coeffs, *pclust_size);

		// Updated to Rprintf to cope with CRAN requirements
		Rprintf("Probabilities of clustering coefficients:\n");

		for(p=0; p<*pclust_size; ++p) {
                        Rprintf("%lf ", prob_clust_coeffs[p]);
                        }
                Rprintf("\n");
		

		/*
   		for(p=0; p<number_genes; ++p) {
       			for(q=0; q<number_genes; ++q) {
				printf("%lf ", neighbour_probs[q+p*number_genes]);
				}
			printf("\n");
			}
		*/
		//tolerance-=5;
	}

	for(p=0; p<number_genes; ++p) {
		hub_probs[p]=probs[p];
		//printf("%lf ", probs[p]);
		for(q=0;q<number_genes;++q) {
			rneighbour_probs[q+p*number_genes]=neighbour_probs[q+p*number_genes];
			//printf("%lf ", neighbour_probs[q+p*number_genes]);
		}
		//printf("\n");
	}

	//printf("\n");

	*number_accepted=hits;

	/*******************************************
 	*frees up the memory
	********************************************/

	for(i=0; i<number_genes; ++i) {
		free(data[i]);
	}
	free(data);

	for(i=0; i<number_genes; ++i) {
		free(simulated_data[i]);
	}
	free(simulated_data);

	for(i=0; i<number_perms; ++i) {
		free(configs[i]);
	}
	free(configs);

	free(omega);
	free(hubs);
	free(nodes);
	free(labels);
	free(probs);
	free(cumul_probs);
	free(degrees);
	network->cleanup(network);
	free(network);
	free(track_labels);
	free(array1);
	free(array2);
	free(neighbour_freqs);
	free(neighbour_probs);
	free(place_holder);
	free(config_probs);
	free(prob_clust_coeffs);
	free(freq_clust_coeffs);

	/**************************/
	PutRNGstate();

}


