cmake_minimum_required(VERSION 3.5)
project(autogen)

set(CMAKE_CXX_STANDARD 20)
include_directories(/home/amour/Documents/epitech/rtype/.tmpCompileSrc/Enemy /home/amour/Documents/epitech/rtype/.tmpCompileSrc/Enemy /home/amour/Documents/epitech/rtype/metadata/include/ /home/amour/Documents/epitech/rtype/ecs/include/ /home/amour/Documents/epitech/rtype/ecs/raylib/src/ /home/amour/Documents/epitech/rtype/ecs/raylib/examples/shapes/ /home/amour/Documents/epitech/rtype/server/include/ /home/amour/Documents/epitech/rtype/game_rtype/scripts  /home/amour/Documents/epitech/rtype/game_rtype )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /home/amour/Documents/epitech/rtype/.tmpCompileSrc/Enemy)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}  ${CMAKE_CURRENT_SOURCE_DIR}/../build/)
link_directories(
)
add_library(Enemy SHARED /home/amour/Documents/epitech/rtype/.tmpCompileSrc/Enemy/Enemy.hpp .././.metadata/EnemyMeta.cpp /home/amour/Documents/epitech/rtype/.tmpCompileSrc/Enemy/Enemy.cpp)
add_compile_options(-g3)