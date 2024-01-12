cmake_minimum_required(VERSION 3.27)
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
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin)

set(SRC
    LocalPlayerController.cpp
    DistantPlayer.cpp
    LobbyCoroutine.cpp
    RTypeDistantServer.cpp
    GameRoutine.cpp
    PlayerFromServerController.cpp
    AIController.cpp
    # scripts/Ship.cpp
    # scripts/Laser.cpp
    # scripts/LaserTcemort.cpp
    # scripts/Heal.cpp
    # scripts/X2.cpp
    # scripts/X3.cpp
    # scripts/Tcemort.cpp
    # scripts/Speed.cpp
    # scripts/Nuke.cpp
    # scripts/Force_ic.cpp
    # scripts/Enemy.cpp
    # scripts/Enemy2.cpp
    # scripts/Ship.cpp
    # scripts/Laser.cpp
    # scripts/Enemy.cpp
    # scripts/EnemySpawner.cpp
    # scripts/Level1Manager.cpp
    # # scripts/RLCamera3D.cpp
    # # scripts/RLMesh.cpp
    # scripts/AudioManager.cpp
    scripts/AudioManager.cpp
    scripts/AudioManager2.cpp
    scripts/Boss.cpp
    scripts/BossBody.cpp
    scripts/BossLaser.cpp
    scripts/Chat.cpp
    scripts/Enemy.cpp
    scripts/Enemy2.cpp
    scripts/EnemySpawner.cpp
    scripts/Force_ic.cpp
    scripts/Force.cpp
    scripts/Heal.cpp
    scripts/Laser.cpp
    scripts/LaserTcemort.cpp
    scripts/Level1Manager.cpp
    scripts/LevelUI.cpp
    scripts/MenuScript.cpp
    scripts/Nuke.cpp
    scripts/Rocket.cpp
    scripts/Ship.cpp
    scripts/Speed.cpp
    scripts/Tcemort.cpp
    scripts/X2.cpp
    scripts/X3.cpp
)

set(META_SRC
    # ../.metadata/AudioManagerMeta.cpp
    # ../.metadata/EnemyMeta.cpp
    # # ../.metadata/EnemyShipControllerMeta.cpp
    # ../.metadata/EnemySpawnerMeta.cpp
    # ../.metadata/LaserMeta.cpp
    # ../.metadata/LaserTcemortMeta.cpp
    # ../.metadata/HealMeta.cpp
    # ../.metadata/X2Meta.cpp
    # ../.metadata/X3Meta.cpp
    # ../.metadata/TcemortMeta.cpp
    # ../.metadata/SpeedMeta.cpp
    # ../.metadata/NukeMeta.cpp
    # ../.metadata/Force_icMeta.cpp
    # ../.metadata/ShipMeta.cpp
    # ../.metadata/EnemyMeta.cpp
    # ../.metadata/Enemy2Meta.cpp

    ../.metadata/AudioManagerMeta.cpp
    # ../.metadata/AudioManager2Meta.cpp
    ../.metadata/BossMeta.cpp
    ../.metadata/BossBodyMeta.cpp
    ../.metadata/BossLaserMeta.cpp
    ../.metadata/ChatMeta.cpp
    ../.metadata/EnemyMeta.cpp
    ../.metadata/Enemy2Meta.cpp
    ../.metadata/EnemySpawnerMeta.cpp
    ../.metadata/Force_icMeta.cpp
    ../.metadata/ForceMeta.cpp
    ../.metadata/HealMeta.cpp
    ../.metadata/LaserMeta.cpp
    ../.metadata/LaserTcemortMeta.cpp
    ../.metadata/Level1ManagerMeta.cpp
    ../.metadata/LevelUIMeta.cpp
    ../.metadata/MenuScriptMeta.cpp
    ../.metadata/NukeMeta.cpp
    ../.metadata/RocketMeta.cpp
    ../.metadata/ShipMeta.cpp
    ../.metadata/SpeedMeta.cpp
    ../.metadata/TcemortMeta.cpp
    ../.metadata/X2Meta.cpp
    ../.metadata/X3Meta.cpp
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

add_library(Ship SHARED scripts/Ship.cpp ../.metadata/ShipMeta.cpp)
set_target_properties(Ship PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(Ship game_target)

if (WIN32)

# add_library(LevelUI SHARED scripts/LevelUI.cpp ../.metadata/LevelUIMeta.cpp)
# set_target_properties(LevelUI PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
# target_link_libraries(LevelUI
#     lengine)

# add_library(RLCamera3D SHARED scripts/RLCamera3D.cpp ../.metadata/RLCamera3DMeta.cpp)
# set_target_properties(RLCamera3D PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
# target_link_libraries(RLCamera3D
#     lengine)

# add_library(RLMesh SHARED scripts/RLMesh.cpp ../.metadata/RLMeshMeta.cpp)
# set_target_properties(RLMesh PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
# target_link_libraries(RLMesh
#     lengine)

add_library(MenuScript SHARED scripts/MenuScript.cpp ../.metadata/MenuScriptMeta.cpp)
set_target_properties(MenuScript PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(MenuScript
    lengine)

endif()


add_library(Enemy2 SHARED scripts/Enemy2.cpp ../.metadata/Enemy2Meta.cpp)
set_target_properties(Enemy2 PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(Enemy2 game_target)

add_library(BossBody SHARED scripts/BossBody.cpp ../.metadata/BossBodyMeta.cpp)
set_target_properties(BossBody PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(BossBody game_target)

add_library(BossLaser SHARED scripts/BossLaser.cpp ../.metadata/BossLaserMeta.cpp)
set_target_properties(BossLaser PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(BossLaser game_target)

add_library(Boss SHARED scripts/Boss.cpp ../.metadata/BossMeta.cpp BossController.cpp)
set_target_properties(Boss PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(Boss game_target BossBody BossLaser)

add_library(Chat SHARED scripts/Chat.cpp ../.metadata/ChatMeta.cpp)
set_target_properties(Chat PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(game_target Chat)

add_library(Force SHARED scripts/Force.cpp ../.metadata/ForceMeta.cpp)
set_target_properties(Force PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(Force game_target)

add_library(Force_ic SHARED scripts/Force_ic.cpp ../.metadata/Force_icMeta.cpp)
set_target_properties(Force_ic PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(Force_ic game_target)

add_library(Heal SHARED scripts/Heal.cpp ../.metadata/HealMeta.cpp)
set_target_properties(Heal PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(Heal game_target)

add_library(LaserTcemort SHARED scripts/LaserTcemort.cpp ../.metadata/LaserTcemortMeta.cpp)
set_target_properties(LaserTcemort PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(LaserTcemort game_target)

add_library(Nuke SHARED scripts/Nuke.cpp ../.metadata/NukeMeta.cpp)
set_target_properties(Nuke PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(Nuke game_target)

add_library(Speed SHARED scripts/Speed.cpp ../.metadata/SpeedMeta.cpp)
set_target_properties(Speed PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(Speed game_target)

add_library(Tcemort SHARED scripts/Tcemort.cpp ../.metadata/TcemortMeta.cpp)
set_target_properties(Tcemort PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(Tcemort game_target)

add_library(X2 SHARED scripts/X2.cpp ../.metadata/X2Meta.cpp)
set_target_properties(X2 PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(X2 game_target)

add_library(X3 SHARED scripts/X3.cpp ../.metadata/X3Meta.cpp)
set_target_properties(X3 PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(X3 game_target)

add_library(Rocket SHARED scripts/Rocket.cpp ../.metadata/RocketMeta.cpp)
set_target_properties(Rocket PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/metabuild)
target_link_libraries(Rocket game_target)
