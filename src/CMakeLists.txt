cmake_minimum_required(VERSION 3.13)
project(src C)

set(CMAKE_C_STANDARD 11)

add_executable(src
        abc_R.c
        graph.c
        graph.h
        graph_adjacency_list.h
        motif.c
        network.h
        network_random_R.c
        pool.c
        R_init_networkABC.c
        pool.h
        utils.c utils.h network_random_R.h motif.h abc_R.h localCC.f90)

include_directories(.)

#Set your directories.  The dot representes the root application folder.
#Thus my the path to my domain folder:
set(APP_R /Library/Frameworks/R.framework/Versions/3.5/Resources/include)
set(APP_R_privheaders /Library/Frameworks/R.framework/Versions/3.5/PrivateHeaders)
set(APP_R_libs /Library/Frameworks/R.framework/Libraries)

#Include the directories and now your cpp files will recognize your headers
include_directories(${APP_R})
include_directories(${APP_R_privheaders})

LINK_DIRECTORIES(${APP_R_libs})
file(GLOB LIBRARIES "/Library/Frameworks/R.framework/Libraries/*.dylib")
message("LIBRARIES = ${LIBRARIES}")

TARGET_LINK_LIBRARIES(src  ${LIBRARIES})


