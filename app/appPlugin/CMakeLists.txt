cmake_minimum_required(VERSION 3.5)
project(rTypeProtocol)


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(SOURCE
    src/App.cpp
    src/main.cpp
)


add_executable(${PROJECT_NAME} ${SOURCE})
set_target_properties(${PROJECT_NAME} PROPERTIES EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
# add_library(${PROJECT_NAME} SHARED ${SOURCE})