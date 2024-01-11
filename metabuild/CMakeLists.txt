cmake_minimum_required(VERSION 3.5)
project(autogen)

set(CMAKE_CXX_STANDARD 20)
include_directories(/home/gxby/Dev/Epitech/tek3/R-Type/RTypeV0.0/.tmpCompileSrc/MenuScript /home/gxby/Dev/Epitech/tek3/R-Type/RTypeV0.0/.tmpCompileSrc/MenuScript /home/gxby/Dev/Epitech/tek3/R-Type/RTypeV0.0/metadata/include/ /home/gxby/Dev/Epitech/tek3/R-Type/RTypeV0.0/ecs/include/ /home/gxby/Dev/Epitech/tek3/R-Type/RTypeV0.0/ecs/raylib/src/ /home/gxby/Dev/Epitech/tek3/R-Type/RTypeV0.0/ecs/raylib/examples/shapes/ /home/gxby/Dev/Epitech/tek3/R-Type/RTypeV0.0/server/include/ /home/gxby/Dev/Epitech/tek3/R-Type/RTypeV0.0/luaEngineApplet/include/ /home/gxby/Dev/Epitech/tek3/R-Type/RTypeV0.0/game_rtype/scripts  /home/gxby/Dev/Epitech/tek3/R-Type/RTypeV0.0/game_rtype  /home/gxby/Dev/Epitech/tek3/R-Type/RTypeV0.0/build/_deps/luabridge-src/Tests/Lua/lua-5.1.5/src  /home/gxby/Dev/Epitech/tek3/R-Type/RTypeV0.0/build/_deps/luabridge-src/Source )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /home/gxby/Dev/Epitech/tek3/R-Type/RTypeV0.0/.tmpCompileSrc/MenuScript)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}  ${CMAKE_CURRENT_SOURCE_DIR}/../build/)
link_directories(
)
add_library(MenuScript SHARED /home/gxby/Dev/Epitech/tek3/R-Type/RTypeV0.0/.tmpCompileSrc/MenuScript/MenuScript.hpp .././.metadata/MenuScriptMeta.cpp /home/gxby/Dev/Epitech/tek3/R-Type/RTypeV0.0/.tmpCompileSrc/MenuScript/MenuScript.cpp)
add_compile_options(-g3)