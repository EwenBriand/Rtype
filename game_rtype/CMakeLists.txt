include_directories(
    ${CMAKE_SOURCE_DIR}
    ./scripts/
)

set(SRC
    LocalPlayerController.cpp
    DistantPlayer.cpp
    scripts/Ship.cpp
    scripts/Laser.cpp
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
