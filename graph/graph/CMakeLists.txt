cmake_minimum_required(VERSION 3.0)
project(graph)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
message(FATAL_ERROR "In-source builds are not allowed. Please run cmake from the 'build' directory.")
endif()

find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
message(STATUS "GLFW3 not found, building from subdirectory")
add_subdirectory(glfw)
install(TARGETS glfw
EXPORT glfwTargets
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
RUNTIME DESTINATION bin
INCLUDES DESTINATION include)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include/GLFW
DESTINATION include)
endif()


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/lightgraph/include
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include/glad)
add_library(glad SHARED glad/src/glad.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(lightgraph)
