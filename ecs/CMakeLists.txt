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
set(SRC
    linenoise/linenoise.c
    src/ECS.cpp
    src/ResourceManager.cpp
    src/InputManager.cpp
    src/Hoverable.cpp
    src/SceneManager.cpp
    src/GenericStorageSystem.cpp
    src/UIManager.cpp
    ./include/Components.Vanilla/INode.cpp
)
set(CMAKE_CXX_STANDARD 17)

set(
    METADATA_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/../build/metabuild/
    )

add_library(
    raylibgraph SHARED
    src/GraphicalRayLib.cpp
)

target_link_libraries(raylibgraph
    raylib)

set(CMAKE_CXX_STANDARD 20)
add_library(
    ecs SHARED
    ${SRC}
)

add_library(
    engine SHARED
    src/Engine.cpp
    src/ScopeLocation.cpp
    # ./include/Components.Vanilla/WowGraphics.cpp
)

target_link_libraries(engine
    ecs
    metadatagen
    # lightgraph
)

add_executable(testtarget
    src/test_main.cpp)

# manually build components that do not have metadata
add_library(
    cli SHARED
    ./include/Components.Vanilla/CLI.cpp
    ./include/Components.Vanilla/INode.cpp
)


add_library(
    server SHARED
    ./src/Server.cpp
    ./src/ServerImpl.cpp
    ./src/CircularBuffer.cpp
    ./src/ClientBucket.cpp
    ./src/Message.cpp
)

find_package(Threads REQUIRED)
add_executable(
    server_test_main
    ./src/server_test_main.cpp
)
target_link_libraries(
    server_test_main
    server
    Threads::Threads
)

add_library(
    usercomponentwrapper SHARED
    ./include/Components.Vanilla/UserComponentWrapper.cpp
)

add_library(
    editorMouseManager SHARED
    ./include/Components.Vanilla/EditorMouseManager.cpp
)

target_link_libraries(
    editorMouseManager
    m
)

add_library(
    entityExplorer SHARED
    ./include/Components.Vanilla/EntityExplorer.cpp
)

target_link_libraries(
    entityExplorer
    ${METADATA_SRC}/libTextField.so
)


# link libraries for the components that have metadata
target_link_libraries(ecs
    raylibgraph
    ${METADATA_SRC}/libCoreTransform.so
    ${METADATA_SRC}/libEventGraph.so
    ${METADATA_SRC}/libNodePrintStr.so
    ${METADATA_SRC}/libNodeStr.so
    ${METADATA_SRC}/libUIDiv.so
    ${METADATA_SRC}/libUIButton.so
    ${METADATA_SRC}/libUIMenu.so
    ${METADATA_SRC}/libTextField.so
    ${METADATA_SRC}/libAnimator.so
    ${METADATA_SRC}/libCollider2D.so
    ${METADATA_SRC}/libAudioSource.so
    editorMouseManager
    usercomponentwrapper
    entityExplorer
    cli
    server
)


target_link_libraries(testtarget engine)


# tests
add_executable(
    test_generic_storage_system
    ./src/test_generic_storage_system.cpp
    )

target_link_libraries(
    test_generic_storage_system
    engine
    ecs
)
add_test(
    NAME test_generic_storage_system
    COMMAND test_generic_storage_system
)
