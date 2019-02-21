/*
 *Represents a structure for a complete genetic network.
 * 2015. Author K.Musayeva <khmusayeva@gmail.com>
 * 2019. Fixes and updates by F. Bertrand <fbertran@math.unistra.fr>
 * Copyright (c) Universite de Strasbourg and CNRS
 */

#include "graph.h"
#include "pool.h"

typedef struct Network_str {
        int *V;
        int *H;
        Graph *G;
        Pool *pool;
        int number_of_nodes;
        void (*initV)();
        void (*initH)();
        void (*initGraph)();
        void (*initPool)();
        void (*updateV)(struct Network_str *self, int n);
        void (*shuffleV)(struct Network_str *self);
        void (*cleanup)(struct Network_str *self);
        void (*resetNetwork)(struct Network_str *self);
        void (*generate)(struct Network_str *self, int n, int m, double c);
} Network;

