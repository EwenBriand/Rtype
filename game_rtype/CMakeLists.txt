include_directories(${CMAKE_SOURCE_DIR})

add_library(game_target SHARED
    GameRtype.cpp
)
target_link_libraries(game_target
    lengine)
