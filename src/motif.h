//
// 2019-02-18. Author F. Bertrand <frederic.bertrand@utt.fr>
// Copyright (c) Universite de Strasbourg and CNRS
//

#ifndef SRC_MOTIF_H
#define SRC_MOTIF_H

#include "graph.h"

Graph* generateSIM(int precise_number_nodes, int max_nodes);
Graph* generateDOR(int precise_number_nodes, int max_nodes);
Graph* generateFL(int precise_number_nodes, int max_nodes);

#endif //SRC_MOTIF_H
