cmake_minimum_required(VERSION 3.15)
project(ECS)

include(CTest)

set(CMAKE_CXX_STANDARD 17)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
message(FATAL_ERROR "In-source builds are not allowed. Please run cmake from the 'build' directory.")
endif()

set(CMAKE_INSTALL_RPATH "$ORIGIN")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS "Using ccache")
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
    set(CMAKE_C_COMPILER_LAUNCHER ccache)
else()
    message(WARNING "ccache not found; build may be slower. Install ccache for faster builds.")
endif()


FetchContent_Declare(
  lua
  GIT_REPOSITORY https://github.com/lua/lua.git
  GIT_TAG v5.4.3
)
FetchContent_GetProperties(lua)
if(NOT lua_POPULATED)
  FetchContent_Populate(lua)
  set(LUA_SOURCES
    ${lua_SOURCE_DIR}/lapi.c
    ${lua_SOURCE_DIR}/lcode.c
    ${lua_SOURCE_DIR}/lctype.c
    ${lua_SOURCE_DIR}/ldebug.c
    ${lua_SOURCE_DIR}/ldo.c
    ${lua_SOURCE_DIR}/ldump.c
    ${lua_SOURCE_DIR}/lfunc.c
    ${lua_SOURCE_DIR}/lgc.c
    ${lua_SOURCE_DIR}/llex.c
    ${lua_SOURCE_DIR}/lmem.c
    ${lua_SOURCE_DIR}/lobject.c
    ${lua_SOURCE_DIR}/lopcodes.c
    ${lua_SOURCE_DIR}/lparser.c
    ${lua_SOURCE_DIR}/lstate.c
    ${lua_SOURCE_DIR}/lstring.c
    ${lua_SOURCE_DIR}/ltable.c
    ${lua_SOURCE_DIR}/ltm.c
    ${lua_SOURCE_DIR}/lundump.c
    ${lua_SOURCE_DIR}/lvm.c
    ${lua_SOURCE_DIR}/lzio.c
    ${lua_SOURCE_DIR}/lauxlib.c
    ${lua_SOURCE_DIR}/lbaselib.c
    ${lua_SOURCE_DIR}/lcorolib.c
    ${lua_SOURCE_DIR}/ldblib.c
    ${lua_SOURCE_DIR}/liolib.c
    ${lua_SOURCE_DIR}/lmathlib.c
    ${lua_SOURCE_DIR}/loslib.c
    ${lua_SOURCE_DIR}/lstrlib.c
    ${lua_SOURCE_DIR}/ltablib.c
    ${lua_SOURCE_DIR}/lutf8lib.c
    ${lua_SOURCE_DIR}/loadlib.c
    ${lua_SOURCE_DIR}/linit.c
  )
  add_library(lua SHARED ${LUA_SOURCES})
  include_directories(${lua_SOURCE_DIR})
endif ()

FetchContent_Declare(
    luabridge
    GIT_REPOSITORY https://github.com/vinniefalco/LuaBridge.git
    GIT_TAG master
)
FetchContent_GetProperties(luabridge)
if(NOT luabridge_POPULATED)
  FetchContent_Populate(luabridge)
  message(STATUS "luabridge_SOURCE_DIR: ${luabridge_SOURCE_DIR}")
  include_directories(${luabridge_SOURCE_DIR}/Source)
endif()
include_directories(${luabridge_SOURCE_DIR}/Source)


FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG master
)
FetchContent_GetProperties(yaml-cpp)

if(NOT yaml-cpp_POPULATED)
  message(STATUS "Fetching yaml-cpp...")

  FetchContent_Populate(yaml-cpp)
  set(YAML_BUILD_SHARED_LIBS ON CACHE BOOL "Enable compilation of shared libraries")
  add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()
FetchContent_MakeAvailable(yaml-cpp)


set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS ON)
set(CMAKE_BUILD_TYPE Release)


if(NOT DEFINED BUILD_SHARED_LIBS)
set(BUILD_SHARED_LIBS ON)
endif()
add_compile_options(-O3)

include_directories(server/include)
include_directories(ecs/include)
include_directories(ecs/include/Components.Vanilla)
include_directories(ecs/include/GraphicalRayLib)
include_directories(ecs/raylib/src)
include_directories(metadata/include)
include_directories(luaEngineApplet/include)

add_subdirectory(metadata)
add_subdirectory(ecs/raylib/)
add_subdirectory(server)
add_subdirectory(luaEngineApplet)

add_custom_target(call_metagen ALL
    DEPENDS MetaData generate_metadata
)

if(WIN32)
add_custom_command(OUTPUT MetaData
    # COMMAND echo "-- Generating Metadata from Vanilla Components"
    # COMMAND metadata\\generate_metadata.exe ${CMAKE_SOURCE_DIR}\\ecs\\include\\Components.Vanilla
    # COMMAND echo "-- Resuming build"
    # DEPENDS always_rebuild
    COMMAND echo "-- Generate Metadata manually then restart"
)
else()
add_custom_command(OUTPUT MetaData
    # COMMAND echo "-- Generating Metadata from Vanilla Components"
    # COMMAND ./metadata/generate_metadata ${CMAKE_SOURCE_DIR}/ecs/include/Components.Vanilla/
    # COMMAND echo "-- Resuming build"
    DEPENDS always_rebuild
)

endif()

add_custom_command(OUTPUT always_rebuild
COMMAND echo ""
)

add_custom_target(install_fast
    COMMAND sudo cp ${CMAKE_SOURCE_DIR}/fast /usr/local/bin/fast
)

add_custom_target(doc
    COMMAND doxygen ../Doxyfile
)

add_subdirectory(ecs)
add_subdirectory(game_rtype)

add_subdirectory(tests)
add_subdirectory(game_rtype/plugins/LagChecker)

if(WIN32)
    set(CPACK_GENERATOR "NSIS")
endif()

install(TARGETS run_engine lengine server_udp metadatagen raylib lua yaml-cpp
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION bin)

include(CPack)
set(CPACK_PACKAGE_NAME "REngine")
set(CPACK_PACKAGE_VENDOR "OuiOuiCorp")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "REngine - A  CLI Game Engine")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_INSTALLDIRECTORY "REngine${CPACK_PACKAGE_VERSION}")
