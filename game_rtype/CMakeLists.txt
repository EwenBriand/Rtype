include_directories(
    ${CMAKE_SOURCE_DIR}
    ./scripts/
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")

set(SRC
    LocalPlayerController.cpp
    DistantPlayer.cpp
    LobbyCoroutine.cpp
    RTypeDistantServer.cpp
    scripts/Ship.cpp
    GameRoutine.cpp
    PlayerFromServerController.cpp
)

set(META_SRC
    ../.metadata/ShipMeta.cpp)



add_library(game_target SHARED
    GameRtype.cpp
    ${SRC}
    ${META_SRC}
)
target_link_libraries(game_target
    lengine)
