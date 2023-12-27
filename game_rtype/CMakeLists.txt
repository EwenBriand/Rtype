include_directories(
    ${CMAKE_SOURCE_DIR}
    ./scripts/
    ./
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")

set(SRC
    LocalPlayerController.cpp
    DistantPlayer.cpp
    LobbyCoroutine.cpp
    RTypeDistantServer.cpp
    scripts/Ship.cpp
    scripts/Laser.cpp
    GameRoutine.cpp
    PlayerFromServerController.cpp
    AIController.cpp
)

set(META_SRC
    ../.metadata/LaserMeta.cpp
    ../.metadata/ShipMeta.cpp
)

add_library(game_target SHARED
    GameRtype.cpp
    ${SRC}
    ${META_SRC}
)
target_link_libraries(game_target
    lengine)

# manually building some scripts until we have a proper build system
add_library(EnemySpawner SHARED
    scripts/EnemySpawner.cpp
    ../.metadata/EnemySpawnerMeta.cpp
)

set_target_properties(EnemySpawner PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild
)

target_link_libraries(EnemySpawner
    game_target)
