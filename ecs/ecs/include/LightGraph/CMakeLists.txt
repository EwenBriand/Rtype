cmake_minimum_required(VERSION 3.15)
project(lightgraphmodule)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ../
    ./include
    ./../../../graph/lightgraph/include
    ./../../../graph/glad/include/glad
    ../../../metadata/include
)

set(SRC
    LightGraph.cpp
)


add_library(
    lightgraphmodule
    SHARED
    ${SRC}
)

target_link_libraries(
    lightgraphmodule
    lightgraph
)
