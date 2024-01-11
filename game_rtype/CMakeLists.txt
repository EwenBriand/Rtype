include_directories(
    ${CMAKE_SOURCE_DIR}
    ./scripts/
    ./
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin)

set(SRC
    LocalPlayerController.cpp
    DistantPlayer.cpp
    LobbyCoroutine.cpp
    RTypeDistantServer.cpp
    scripts/Ship.cpp
    scripts/Laser.cpp
    scripts/Enemy.cpp
    scripts/Enemy2.cpp
    GameRoutine.cpp
    PlayerFromServerController.cpp
    AIController.cpp
)

set(META_SRC
    ../.metadata/LaserMeta.cpp
    ../.metadata/ShipMeta.cpp
    ../.metadata/EnemyMeta.cpp
    ../.metadata/Enemy2Meta.cpp

)

add_library(game_target SHARED
    GameRtype.cpp
    ${SRC}
    ${META_SRC}
)
target_link_libraries(game_target
    lengine)

# manually building some scripts until we have a proper build system
add_library(EnemySpawner SHARED scripts/EnemySpawner.cpp ../.metadata/EnemySpawnerMeta.cpp)
set_target_properties(EnemySpawner PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(EnemySpawner game_target)

add_library(Enemy SHARED scripts/Enemy.cpp ../.metadata/EnemyMeta.cpp)
set_target_properties(Enemy PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(Enemy game_target)

add_library(Enemy2 SHARED scripts/Enemy2.cpp ../.metadata/Enemy2Meta.cpp)
set_target_properties(Enemy2 PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(Enemy2 game_target)


add_library(Chat SHARED scripts/Chat.cpp ../.metadata/ChatMeta.cpp)
set_target_properties(Chat PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(game_target Chat)

add_library(Block SHARED scripts/Block.cpp ../.metadata/BlockMeta.cpp)
set_target_properties(Block PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(Block game_target)

add_library(BlockSpawner SHARED scripts/BlockSpawner.cpp ../.metadata/BlockSpawnerMeta.cpp)
set_target_properties(BlockSpawner PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(BlockSpawner game_target)
target_link_libraries(BlockSpawner Block)