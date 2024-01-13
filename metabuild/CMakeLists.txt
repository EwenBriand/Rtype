cmake_minimum_required(VERSION 3.5)
project(autogen)

set(CMAKE_CXX_STANDARD 20)
include_directories(/home/amour/Documents/epitech/finalClone/.tmpCompileSrc/X3 /home/amour/Documents/epitech/finalClone/.tmpCompileSrc/X3 /home/amour/Documents/epitech/finalClone/metadata/include/ /home/amour/Documents/epitech/finalClone/ecs/include/ /home/amour/Documents/epitech/finalClone/ecs/raylib/src/ /home/amour/Documents/epitech/finalClone/ecs/raylib/examples/shapes/ /home/amour/Documents/epitech/finalClone/server/include/ /home/amour/Documents/epitech/finalClone/luaEngineApplet/include/ /home/amour/Documents/epitech/finalClone/game_rtype/scripts  /home/amour/Documents/epitech/finalClone/game_rtype  /home/amour/Documents/epitech/finalClone/build/_deps/luabridge-src/Tests/Lua/lua-5.1.5/src  /home/amour/Documents/epitech/finalClone/build/_deps/luabridge-src/Source )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /home/amour/Documents/epitech/finalClone/.tmpCompileSrc/X3)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}  ${CMAKE_CURRENT_SOURCE_DIR}/../build/)
link_directories(
)
add_library(X3 SHARED /home/amour/Documents/epitech/finalClone/.tmpCompileSrc/X3/X3.hpp .././.metadata/X3Meta.cpp /home/amour/Documents/epitech/finalClone/.tmpCompileSrc/X3/X3.cpp)
add_compile_options(-g3)