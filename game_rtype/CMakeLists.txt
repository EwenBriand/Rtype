cmake_minimum_required(VERSION 3.28)
project(game_target)

# find_library(LIBRARY_ENGINE lengine PATHS build/ecs)
# find_library(LIBRARY_SERVER server_udp PATHS build/server)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ./scripts/
    ../server/include/
    ../ecs/include/
    ../ecs/include/Components.Vanilla/
    ../ecs/raylib/src/
    ../metadata/include/
    ./
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")

set(SRC
    LocalPlayerController.cpp
    DistantPlayer.cpp
    LobbyCoroutine.cpp
    RTypeDistantServer.cpp
    GameRoutine.cpp
    PlayerFromServerController.cpp
    AIController.cpp
    scripts/Ship.cpp
    scripts/Laser.cpp
    scripts/Enemy.cpp
    scripts/EnemySpawner.cpp
    scripts/Level1Manager.cpp
    # scripts/RLCamera3D.cpp
    # scripts/RLMesh.cpp
    scripts/AudioManager.cpp
)

set(META_SRC
    ../.metadata/AudioManagerMeta.cpp
    ../.metadata/EnemyMeta.cpp
    # ../.metadata/EnemyShipControllerMeta.cpp
    ../.metadata/EnemySpawnerMeta.cpp
    ../.metadata/LaserMeta.cpp
    ../.metadata/Level1ManagerMeta.cpp
    # ../.metadata/LocalPlayerControllerMeta.cpp
    # ../.metadata/MenuScriptMeta.cpp
    # ../.metadata/RLCamera3DMeta.cpp
    # ../.metadata/RLMeshMeta.cpp
    ../.metadata/ShipMeta.cpp
)

add_library(game_target SHARED
    GameRtype.cpp
    ${SRC}
    ${META_SRC}
)


target_link_libraries(game_target
    lengine
    server_udp)

# manually building some scripts until we have a proper build system
add_library(AudioManager SHARED scripts/AudioManager.cpp ../.metadata/AudioManagerMeta.cpp)
set_target_properties(AudioManager PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(AudioManager game_target)

add_library(Enemy SHARED scripts/Enemy.cpp ../.metadata/EnemyMeta.cpp)
set_target_properties(Enemy PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(Enemy game_target)

add_library(EnemySpawner SHARED scripts/EnemySpawner.cpp ../.metadata/EnemySpawnerMeta.cpp)
set_target_properties(EnemySpawner PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(EnemySpawner game_target)

add_library(Laser SHARED scripts/Laser.cpp ../.metadata/LaserMeta.cpp)
set_target_properties(Laser PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(Laser game_target)

add_library(Level1Manager SHARED scripts/Level1Manager.cpp ../.metadata/Level1ManagerMeta.cpp)
set_target_properties(Level1Manager PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(Level1Manager game_target)

# add_library(LevelUI SHARED scripts/LevelUI.cpp ../.metadata/LevelUIMeta.cpp)
# set_target_properties(LevelUI PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
# target_link_libraries(LevelUI game_target)

# add_library(RLCamera3D SHARED scripts/RLCamera3D.cpp ../.metadata/RLCamera3DMeta.cpp)
# set_target_properties(RLCamera3D PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
# target_link_libraries(RLCamera3D game_target)

# add_library(RLMesh SHARED scripts/RLMesh.cpp ../.metadata/RLMeshMeta.cpp)
# set_target_properties(RLMesh PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
# target_link_libraries(RLMesh game_target)

add_library(Ship SHARED scripts/Ship.cpp ../.metadata/ShipMeta.cpp)
set_target_properties(Ship PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(Ship game_target)
