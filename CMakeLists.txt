# cmake_minimum_required(VERSION 3.15)
# project(test_raytracing)

cmake_minimum_required(VERSION 3.15)
project(ECS)

set(CMAKE_CXX_STANDARD 17)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
message(FATAL_ERROR "In-source builds are not allowed. Please run cmake from the 'build' directory.")
endif()

if(NOT DEFINED BUILD_SHARED_LIBS)
set(BUILD_SHARED_LIBS ON)
endif()
add_compile_options(-O3)

add_subdirectory(metadata)
add_subdirectory(ecs)
add_subdirectory(ecs/raylib/)
add_subdirectory(graph)

add_custom_target(call_metagen ALL
    DEPENDS MetaData generate_metadata
    )

    add_custom_command(OUTPUT MetaData
    COMMAND echo "-- Generating Metadata from Vanilla Components" && ./metadata/generate_metadata ${CMAKE_SOURCE_DIR}/ecs/include/Components.Vanilla/
    COMMAND echo "-- Resuming build"
    DEPENDS always_rebuild
)

add_custom_command(OUTPUT always_rebuild
COMMAND echo ""
)
