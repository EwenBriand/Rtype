cmake_minimum_required(VERSION 3.5)
project(autogen)

set(CMAKE_CXX_STANDARD 20)
include_directories(/home/amour/Documents/epitech/safe/.tmpCompileSrc/Ball /home/amour/Documents/epitech/safe/.tmpCompileSrc/Ball /home/amour/Documents/epitech/safe/metadata/include/ /home/amour/Documents/epitech/safe/ecs/include/ /home/amour/Documents/epitech/safe/ecs/raylib/src/ /home/amour/Documents/epitech/safe/ecs/raylib/examples/shapes/ /home/amour/Documents/epitech/safe/server/include/ /home/amour/Documents/epitech/safe/luaEngineApplet/include/ /home/amour/Documents/epitech/safe/game_pool/scripts  /home/amour/Documents/epitech/safe/game_pool  /home/amour/Documents/epitech/safe/build/_deps/luabridge-src/Tests/Lua/lua-5.1.5/src  /home/amour/Documents/epitech/safe/build/_deps/luabridge-src/Source )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /home/amour/Documents/epitech/safe/.tmpCompileSrc/Ball)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}  ${CMAKE_CURRENT_SOURCE_DIR}/../build/)
link_directories(
)
add_library(Ball SHARED /home/amour/Documents/epitech/safe/.tmpCompileSrc/Ball/Ball.hpp .././.metadata/BallMeta.cpp /home/amour/Documents/epitech/safe/.tmpCompileSrc/Ball/Ball.cpp)
add_compile_options(-g3)