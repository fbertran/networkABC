/**
 * This file provides the functionality of generating 3 main types of motifs.
 * 2015. Author K.Musayeva <khmusayeva@gmail.com>
 * 2019. Fixes and updates by F. Bertrand <fbertran@math.unistra.fr>
 * Copyright (c) Universite de Strasbourg and CNRS
 */

#include <stdlib.h>
//for unif_rand
#include <Rmath.h>
#include "graph.h"
#include "utils.h"
// replaced by unif_rand to cope with CRAN requirements
// #define rand_unit ((double)rand())/RAND_MAX
#define min(x,y) (((x)<(y))? (x) : (y))
#define max(x,y) (((x)<(y))? (y) : (x))
#define PR 0.35 //probability of repression


/**
 * Generates a single-input model (SIM).
 * */
Graph *generateSIM(int precise_number_nodes, int max_nodes) {
    int number_of_nodes=(precise_number_nodes==0)?sample_int(2, max_nodes):precise_number_nodes;
    int regulator_node=sample_int(0, number_of_nodes-1); //randomly chooses a regulator
    int is_repressed;

    //printf("Number of nodes %d\n", number_of_nodes);
    //printf("Regulator node %d\n", regulator_node);

    Graph *graph=createGraph(number_of_nodes, 1); //this is a motif of type 1
    graph->vertices[regulator_node]->is_regulator=1;
    double random_value;
    int i; //, result[number_of_nodes-1]; removed definition due to warning: unused variable 'result' [-Wunused-variable]

    for(i=0; i<number_of_nodes; ++i) {
        if(i==regulator_node) continue;
        random_value=unif_rand();
        is_repressed=(PR>=random_value)?1:0;
        addEdge(regulator_node, i, is_repressed, graph);
    }

    graph->number_of_regulators=1;
    return graph;
}


/**
 * Generates dense overlapping regulon model (DOR).
 * */
Graph *generateDOR(int precise_number_nodes, int max_nodes) {
    int number_of_nodes=(precise_number_nodes==0)?sample_int(3, max_nodes):precise_number_nodes;
    int max_regulators=min(2, number_of_nodes-2);
    //printf("Number of max regulators %d\n", max_regulators);
    //Graph *graph=createGraph(5, 2);

    int number_of_regulators=sample_int(2, max(2,max_regulators));
    int number_of_operons=number_of_nodes-number_of_regulators;
    int is_repressed;

    /*
    printf("Number of nodes %d\n", number_of_nodes);
    printf("Number of regulators %d\n", number_of_regulators);
    printf("Number of operons %d\n", number_of_operons);
    */

    int *regulators=malloc(number_of_regulators*sizeof(int)); //stores the regulator nodes
    int *regulator_operon_size=malloc(number_of_regulators*sizeof(int));//keeps track of how many operons each regulator gets
    int *operons=malloc(number_of_operons*sizeof(int)); //stores operons
    int *orphans=malloc(number_of_nodes*sizeof(int)); //is used to detect the nodes without any edges
    int *array=malloc(number_of_nodes*sizeof(int)); //is used for the random choice of regulators and operons

    int i;
    for(i=0; i<number_of_nodes; ++i){
        array[i]=i;
        orphans[i]=1;//initially no node has an edge
    }

    shuffle(array, number_of_nodes); //randomizes the array

    Graph *graph=createGraph(number_of_nodes, 2);//this is a motif of type 2

    //the first n elements are regulators
    for(i=0; i<number_of_regulators; ++i) {
        regulators[i]=array[i];
        regulator_operon_size[i]=sample_int(1, number_of_operons); //selects random number of nodes for each regulator
        orphans[regulators[i]]=0; //rules out the regulators
        graph->vertices[regulators[i]]->is_regulator=1;
    }

    //the last k elements are operons
    for(i=0; i<number_of_operons; ++i) {
        operons[i]=array[i+number_of_regulators];
    }


    double random_value;
    int index, k, operon, regulator;


    //Only one regulator is labeled as a regulator in a motif. It is needed when joining the disconnected motifs in a network to complete the whole network
    //graph->vertices[regulators[0]]->is_regulator=1;


    //each regulator randomly gets the required number of nodes
    for(i=0; i<number_of_regulators; ++i) {
        regulator=regulators[i];

        //this procedure assures sampling without replacement
        for(k=0; k<regulator_operon_size[i]; k++) {
            index=sample_int(0, number_of_operons-k-1);
            operon=operons[index];
            orphans[operon]=0;
            swap(operons, index, number_of_operons-k-1);
            random_value=unif_rand();
            is_repressed=(PR>=random_value)?1:0;
            addEdge(regulator, operon, is_repressed, graph);
        }

    }


    /**
     * Handles the nodes that haven't been selected by any regulator
     */
    for(i=0;i<number_of_nodes;++i) {
        if(orphans[i]==1) {
            index=sample_int(0, number_of_regulators-1);
            random_value=unif_rand();
            is_repressed=(PR>=random_value)?1:0;
            regulator=regulators[index];
            addEdge(regulator, i, is_repressed, graph);
        }
    }


    graph->number_of_regulators=number_of_regulators;

    //cleanup
    free(regulator_operon_size);
    free(regulators);
    free(operons);
    free(orphans);
    free(array);

    return graph;
}


/**
 * Generates the feedforward loop module (FL).
 */
Graph *generateFL(int precise_number_nodes, int max_nodes) {
    int number_of_nodes=(precise_number_nodes==0)?sample_int(3, max_nodes):precise_number_nodes;
    int is_repressed;

    int *array=malloc(number_of_nodes*sizeof(int)); //used to randomize the structure of the motif

    int i;
    for(i=0;i<number_of_nodes;i++) {
        array[i]=i;
    }

    shuffle(array, number_of_nodes); //randomize the array

    /*
    for(i=0;i<number_of_nodes;i++) {
        printf("%d\n", array[i]);
        }
    */


    Graph *graph=createGraph(number_of_nodes, 3);//this is a motif of type 3
    graph->vertices[array[0]]->is_regulator=1;


    /**
     * Adds edges between the regulator and second node
     */
    double random_value=unif_rand();
    is_repressed=(PR>=random_value)?1:0;
    addEdge(array[0], array[1], is_repressed, graph);


    /**
     * Adds edges between the regulator and the last node.
     */
    random_value=unif_rand();
    is_repressed=(PR>=random_value)?1:0;
    addEdge(array[0], array[number_of_nodes-1], is_repressed, graph);

    for(i=1; i<number_of_nodes-1; ++i) {
        addEdge(array[i], array[i+1], 0, graph);
    }

    graph->number_of_regulators=1;

    free(array);

    return graph;
}


