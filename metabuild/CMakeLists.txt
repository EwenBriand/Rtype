cmake_minimum_required(VERSION 3.5)
project(autogen)

set(CMAKE_CXX_STANDARD 20)
include_directories(/home/ebriand/3year/rtype/RTypeV0.0/.tmpCompileSrc/Ship /home/ebriand/3year/rtype/RTypeV0.0/.tmpCompileSrc/Ship /home/ebriand/3year/rtype/RTypeV0.0/metadata/include/ /home/ebriand/3year/rtype/RTypeV0.0/ecs/include/ /home/ebriand/3year/rtype/RTypeV0.0/ecs/raylib/src/ /home/ebriand/3year/rtype/RTypeV0.0/ecs/raylib/examples/shapes/ /home/ebriand/3year/rtype/RTypeV0.0/server/include/ /home/ebriand/3year/rtype/RTypeV0.0/game_rtype/scripts  /home/ebriand/3year/rtype/RTypeV0.0/game_rtype )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /home/ebriand/3year/rtype/RTypeV0.0/.tmpCompileSrc/Ship)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}  ${CMAKE_CURRENT_SOURCE_DIR}/../build/)
link_directories(
)
add_library(Ship SHARED /home/ebriand/3year/rtype/RTypeV0.0/.tmpCompileSrc/Ship/Ship.hpp .././.metadata/ShipMeta.cpp /home/ebriand/3year/rtype/RTypeV0.0/.tmpCompileSrc/Ship/Ship.cpp)
add_compile_options(-g3)