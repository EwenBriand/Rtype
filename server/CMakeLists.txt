cmake_minimum_required(VERSION 3.5)
project(server_udp)


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/asio/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# if platform is windows
# get boost linkers from C:\boost\lib
# and include boost headers from C:\boost\include\boost-1_84
if(WIN32)
    include_directories(
        C:/boost/include/boost-1_84
    )
    link_directories(
        C:/boost/lib
    )
endif(WIN32)


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
