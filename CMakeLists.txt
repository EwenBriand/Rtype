cmake_minimum_required(VERSION 3.15)
project(ECS)

include(CTest)

set(CMAKE_CXX_STANDARD 17)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
message(FATAL_ERROR "In-source builds are not allowed. Please run cmake from the 'build' directory.")
endif()

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS "Using ccache")
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
    set(CMAKE_C_COMPILER_LAUNCHER ccache)
else()
    message(WARNING "ccache not found; build may be slower. Install ccache for faster builds.")
endif()


set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS ON)
set(CMAKE_BUILD_TYPE Release)


if(NOT DEFINED BUILD_SHARED_LIBS)
set(BUILD_SHARED_LIBS ON)
endif()
add_compile_options(-O3)

include_directories(ecs/include)
include_directories(ecs/include/Components.Vanilla)
include_directories(ecs/raylib/src)
include_directories(metadata/include)


add_subdirectory(metadata)

add_subdirectory(ecs/raylib/)
add_subdirectory(ecs)
# add_subdirectory(tests)
# add_subdirectory(graph)

add_custom_target(call_metagen ALL
    DEPENDS MetaData generate_metadata
    )

    add_custom_command(OUTPUT MetaData
    COMMAND echo "-- Generating Metadata from Vanilla Components"
    
    if(WIN32)
        COMMAND metadata\\generate_metadata.exe ${CMAKE_SOURCE_DIR}\\ecs\\include\\Components.Vanilla
    else()
        COMMAND ./metadata/generate_metadata ${CMAKE_SOURCE_DIR}/ecs/include/Components.Vanilla/
    endif()
    COMMAND echo "-- Resuming build"
    DEPENDS always_rebuild
)

add_custom_command(OUTPUT always_rebuild
COMMAND echo ""
)

add_custom_target(install_fast
    COMMAND sudo cp ${CMAKE_SOURCE_DIR}/fast /usr/local/bin/fast
)

add_custom_target(doc
    COMMAND doxygen ../Doxyfile
)
