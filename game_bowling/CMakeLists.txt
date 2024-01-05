include_directories(
    ${CMAKE_SOURCE_DIR}
    ./scripts/
    ./
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin)

set(SRC
    GameBowling.cpp)

add_library(game_bowling SHARED ${SRC})
target_link_libraries(game_bowling lengine)

# manually build some scripts below if needed