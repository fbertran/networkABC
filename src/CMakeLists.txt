cmake_minimum_required(VERSION 3.13)
project(src C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(src
        abc_R.c
        graph.c
        graph.h
        graph_adjacency_list.h
        motif.c
        network.h
        network_random_R.c
        pool.c
        pool.h
        utils.c utils.h network_random_R.h motif.h)
