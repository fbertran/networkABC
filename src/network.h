/**
 *Represents a structure for a complete genetic network.
 *Copyright (c) 2015 K.Musayeva <khmusayeva@gmail.com>
 */


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

