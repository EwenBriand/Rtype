cmake_minimum_required(VERSION 3.15)
project(ecs)
include(CTest)

set(CMAKE_CXX_STANDARD 20)

# set compile options:
add_compile_options(
    -fmodules-ts
)

include_directories(
    ./include
    ../metadata/include/
    ../include/
    ../graph/lightgraph/include/
    ../graph/glad/include/glad/
    ./linenoise
)

set(CMAKE_LIBRARY_PATH
    ${CMAKE_LIBRARY_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/../build/
    ${CMAKE_CURRENT_SOURCE_DIR}/../ecs/raylib/raylib/
    ${CMAKE_CURRENT_SOURCE_DIR}/ecs/raylib/raylib/
)

link_directories(
    ${CMAKE_LIBRARY_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/../build/
    ${CMAKE_CURRENT_SOURCE_DIR}/../ecs/raylib/raylib/
    ${CMAKE_CURRENT_SOURCE_DIR}/ecs/raylib/raylib/
)

set(CMAKE_CXX_STANDARD 20)

set(ECS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(ECS
    ${ECS_SRC}/../linenoise/linenoise.c
    ${ECS_SRC}/ECS.cpp
    ${ECS_SRC}/LibUtils.cpp
    ${ECS_SRC}/ResourceManager.cpp
    ${ECS_SRC}/InputManager.cpp
    ${ECS_SRC}/Hoverable.cpp
    ${ECS_SRC}/SceneManager.cpp
    ${ECS_SRC}/GenericStorageSystem.cpp
    ${ECS_SRC}/UIManager.cpp
    ${ECS_SRC}/GraphicalRayLib.cpp
    ${ECS_SRC}/AController.cpp
    ${ECS_SRC}/AActor.cpp
)

set(ENGINE
    ${ECS_SRC}/Engine.cpp
    ${ECS_SRC}/ScopeLocation.cpp
)

set(SERVER
    ${ECS_SRC}/Server.cpp
    ${ECS_SRC}/ServerImpl.cpp
    ${ECS_SRC}/CircularBuffer.cpp
    ${ECS_SRC}/ClientBucket.cpp
    ${ECS_SRC}/Message.cpp
    ${ECS_SRC}/Poll.cpp
)

set(VANILLA_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Components.Vanilla)

set(VANILLA
    ${VANILLA_SRC}/Animator.cpp
    ${VANILLA_SRC}/AudioSource.cpp
    ${VANILLA_SRC}/CLI.cpp
    ${VANILLA_SRC}/Collider2D.cpp
    ${VANILLA_SRC}/CoreTransform.cpp
    ${VANILLA_SRC}/EditorMouseManager.cpp
    ${VANILLA_SRC}/EntityExplorer.cpp
    ${VANILLA_SRC}/RigidBody2D.cpp
    ${VANILLA_SRC}/TextField.cpp
    ${VANILLA_SRC}/UIButton.cpp
    ${VANILLA_SRC}/UIDiv.cpp
    ${VANILLA_SRC}/UIMenu.cpp
    ${VANILLA_SRC}/UIScrollableList.cpp
    ${VANILLA_SRC}/UserComponentWrapper.cpp
    ${VANILLA_SRC}/Tag.cpp
)

set(META_VANILLA_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/../build/.metadata
)

set(META_VANILLA
    ${META_VANILLA_SRC}/AnimatorMeta.cpp
    ${META_VANILLA_SRC}/Collider2DMeta.cpp
    ${META_VANILLA_SRC}/EventGraphMeta.cpp
    ${META_VANILLA_SRC}/NodeStrMeta.cpp
    ${META_VANILLA_SRC}/TextFieldMeta.cpp
    ${META_VANILLA_SRC}/UIDivMeta.cpp
    ${META_VANILLA_SRC}/AudioSourceMeta.cpp
    ${META_VANILLA_SRC}/CoreTransformMeta.cpp
    ${META_VANILLA_SRC}/NodePrintStrMeta.cpp
    ${META_VANILLA_SRC}/RigidBody2DMeta.cpp
    ${META_VANILLA_SRC}/UIButtonMeta.cpp
    ${META_VANILLA_SRC}/UIMenuMeta.cpp
    ${META_VANILLA_SRC}/TagMeta.cpp
)

add_library(lengine SHARED
    ${ECS}
    ${ENGINE}
    ${SERVER}
    ${VANILLA}
    ${META_VANILLA}
)

target_link_libraries(lengine
    metadatagen
    raylib
)

add_dependencies(lengine
    call_metagen
)

if(WIN32)
    target_link_libraries(lengine
        ws2_32
    )
endif()

add_executable(run_engine
    src/test_main.cpp)

target_link_libraries(run_engine
    lengine

)
