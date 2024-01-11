cmake_minimum_required(VERSION 3.5)
project(autogen)

set(CMAKE_CXX_STANDARD 20)
include_directories(/home/laurentjiang/delivery/tek3/RTypeV0.0/.tmpCompileSrc/Ship /home/laurentjiang/delivery/tek3/RTypeV0.0/.tmpCompileSrc/Ship /home/laurentjiang/delivery/tek3/RTypeV0.0/metadata/include/ /home/laurentjiang/delivery/tek3/RTypeV0.0/ecs/include/ /home/laurentjiang/delivery/tek3/RTypeV0.0/ecs/raylib/src/ /home/laurentjiang/delivery/tek3/RTypeV0.0/ecs/raylib/examples/shapes/ /home/laurentjiang/delivery/tek3/RTypeV0.0/server/include/ /home/laurentjiang/delivery/tek3/RTypeV0.0/game_rtype/scripts  /home/laurentjiang/delivery/tek3/RTypeV0.0/game_rtype )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /home/laurentjiang/delivery/tek3/RTypeV0.0/.tmpCompileSrc/Ship)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}  ${CMAKE_CURRENT_SOURCE_DIR}/../build/)
link_directories(
)
add_library(Ship SHARED /home/laurentjiang/delivery/tek3/RTypeV0.0/.tmpCompileSrc/Ship/Ship.hpp .././.metadata/ShipMeta.cpp /home/laurentjiang/delivery/tek3/RTypeV0.0/.tmpCompileSrc/Ship/Ship.cpp)
add_compile_options(-g3)