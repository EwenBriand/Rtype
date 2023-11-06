cmake_minimum_required(VERSION 3.15)
project(generate_metadata)

set(CMAKE_CXX_STANDARD 17)
set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(./include)

set(SOURCE_METADATA_GENERATOR
    ${SRC}/main.cpp
    ${SRC}/metadataGenerator.cpp
)

add_executable(generate_metadata
    ${SOURCE_METADATA_GENERATOR}
)

add_library(
    metadatagen
    SHARED
    ${SRC}/metadataGenerator.cpp
)
