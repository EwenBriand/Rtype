cmake_minimum_required(VERSION 3.5)
project(server_udp)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/align/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/asio/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/assert/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/config/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/conversion/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/core/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/date_time/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/numeric_conversion/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/predef/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/smart_ptr/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/static_assert/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/system/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/throw_exception/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/type_traits/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/utility/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/winapi/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")

set(SOURCE
    src/ServerUdp.cpp
    src/ClientUDP.cpp
    src/CircularBuffer.cpp
    src/Coroutine.cpp
    src/Instruction.cpp
    src/Bytes.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCE})
