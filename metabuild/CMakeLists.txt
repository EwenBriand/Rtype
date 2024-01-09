
cmake_minimum_required(VERSION 3.5)
project(autogen)

set(CMAKE_CXX_STANDARD 20)
include_directories(/home/clement_t/Rtype/2Rtype/.tmpCompileSrc/Ship /home/clement_t/Rtype/2Rtype/.tmpCompileSrc/Ship /home/clement_t/Rtype/2Rtype/metadata/include/ /home/clement_t/Rtype/2Rtype/ecs/include/ /home/clement_t/Rtype/2Rtype/ecs/raylib/src/ /home/clement_t/Rtype/2Rtype/ecs/raylib/examples/shapes/ /home/clement_t/Rtype/2Rtype/server/include/ /home/clement_t/Rtype/2Rtype/luaEngineApplet/include/ /home/clement_t/Rtype/2Rtype/game_rtype/scripts  /home/clement_t/Rtype/2Rtype/game_rtype  /home/clement_t/Rtype/2Rtype/build/_deps/luabridge-src/Tests/Lua/lua-5.1.5/src  /home/clement_t/Rtype/2Rtype/build/_deps/luabridge-src/Source )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /home/clement_t/Rtype/2Rtype/.tmpCompileSrc/Ship)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}  ${CMAKE_CURRENT_SOURCE_DIR}/../build/)
link_directories(
)
add_library(Ship SHARED /home/clement_t/Rtype/2Rtype/.tmpCompileSrc/Ship/Ship.hpp .././.metadata/ShipMeta.cpp /home/clement_t/Rtype/2Rtype/.tmpCompileSrc/Ship/Ship.cpp)
add_compile_options(-g3)

