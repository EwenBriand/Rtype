cmake_minimum_required(VERSION 3.5)
project(autogen)

set(CMAKE_CXX_STANDARD 20)
include_directories(/home/ebriand/3year/rtype/RTypeV0.0/.tmpCompileSrc/AudioManager2 /home/ebriand/3year/rtype/RTypeV0.0/.tmpCompileSrc/AudioManager2 /home/ebriand/3year/rtype/RTypeV0.0/metadata/include/ /home/ebriand/3year/rtype/RTypeV0.0/ecs/include/ /home/ebriand/3year/rtype/RTypeV0.0/ecs/raylib/src/ /home/ebriand/3year/rtype/RTypeV0.0/ecs/raylib/examples/shapes/ /home/ebriand/3year/rtype/RTypeV0.0/server/include/ /home/ebriand/3year/rtype/RTypeV0.0/luaEngineApplet/include/ /home/ebriand/3year/rtype/RTypeV0.0/game_rtype/scripts  /home/ebriand/3year/rtype/RTypeV0.0/game_rtype  /home/ebriand/3year/rtype/RTypeV0.0/build/_deps/luabridge-src/Tests/Lua/lua-5.1.5/src  /home/ebriand/3year/rtype/RTypeV0.0/build/_deps/luabridge-src/Source )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /home/ebriand/3year/rtype/RTypeV0.0/.tmpCompileSrc/AudioManager2)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}  ${CMAKE_CURRENT_SOURCE_DIR}/../build/)
link_directories(
)
add_library(AudioManager2 SHARED /home/ebriand/3year/rtype/RTypeV0.0/.tmpCompileSrc/AudioManager2/AudioManager2.hpp .././.metadata/AudioManager2Meta.cpp /home/ebriand/3year/rtype/RTypeV0.0/.tmpCompileSrc/AudioManager2/AudioManager2.cpp)
add_compile_options(-g3)