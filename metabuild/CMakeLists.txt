cmake_minimum_required(VERSION 3.5)
project(autogen)

set(CMAKE_CXX_STANDARD 20)
include_directories(/home/amour/Documents/epitech/rtype/.tmpCompileSrc/RLMesh /home/amour/Documents/epitech/rtype/.tmpCompileSrc/RLMesh /home/amour/Documents/epitech/rtype/metadata/include/ /home/amour/Documents/epitech/rtype/ecs/include/ /home/amour/Documents/epitech/rtype/ecs/raylib/src/ /home/amour/Documents/epitech/rtype/ecs/raylib/examples/shapes/ )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /home/amour/Documents/epitech/rtype/.tmpCompileSrc/RLMesh)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}  ${CMAKE_CURRENT_SOURCE_DIR}/../build/)
link_directories(
)
add_library(RLMesh SHARED /home/amour/Documents/epitech/rtype/.tmpCompileSrc/RLMesh/RLMesh.hpp .././.metadata/RLMeshMeta.cpp /home/amour/Documents/epitech/rtype/.tmpCompileSrc/RLMesh/RLMesh.cpp)
add_compile_options(-g3)