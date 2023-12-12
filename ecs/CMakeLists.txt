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
    ${ECS_SRC}/ECS.cpp
    ${ECS_SRC}/LibUtils.cpp
    ${ECS_SRC}/ResourceManager.cpp
    ${ECS_SRC}/InputManager.cpp
    ${ECS_SRC}/Hoverable.cpp
    ${ECS_SRC}/SceneManager.cpp
    ${ECS_SRC}/GenericStorageSystem.cpp
    ${ECS_SRC}/UIManager.cpp
    ${ECS_SRC}/GraphicalRayLib.cpp
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
)

set(META_VANILLA_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/../.metadata
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
)

add_library(lengine STATIC
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

# add_library(
#     raylibgraph SHARED
#     src/GraphicalRayLib.cpp
# )

# target_link_libraries(raylibgraph
#     engine
#     raylib)

# set(CMAKE_CXX_STANDARD 20)
# add_library(
#     ecs SHARED
#     ${SRC}
# )

# add_library(
#     engine SHARED
#     src/Engine.cpp
#     src/ScopeLocation.cpp
#     # ./include/Components.Vanilla/WowGraphics.cpp
# )

# target_link_libraries(engine
#     ecs
#     metadatagen
#     # lightgraph
# )

# add_executable(testtarget
#     src/test_main.cpp)

# # manually build components that do not have metadata
# add_library(
#     cli SHARED
#     ./include/Components.Vanilla/CLI.cpp
#     # ./include/Components.Vanilla/INode.cpp
# )


# add_library(
#     server SHARED
#     ./src/Server.cpp
#     ./src/ServerImpl.cpp
#     ./src/CircularBuffer.cpp
#     ./src/ClientBucket.cpp
#     ./src/Message.cpp
# )

# find_package(Threads REQUIRED)
# add_executable(
#     server_test_main
#     ./src/server_test_main.cpp
# )
# target_link_libraries(
#     server_test_main
#     server
#     Threads::Threads
# )

# add_library(
#     usercomponentwrapper SHARED
#     ./include/Components.Vanilla/UserComponentWrapper.cpp
# )

# target_link_libraries(
#     usercomponentwrapper
#     cli
# )

# add_library(
#     editorMouseManager SHARED
#     ./include/Components.Vanilla/EditorMouseManager.cpp
# )

# target_link_libraries(
#     editorMouseManager
#     m
# )

# add_library(
#     entityExplorer SHARED
#     ./include/Components.Vanilla/EntityExplorer.cpp
# )

# target_link_libraries(
#     entityExplorer
#     ${METADATA_SRC}/libTextField.so
# )


# # link libraries for the components that have metadata
# target_link_libraries(ecs
#     ${METADATA_SRC}/libCoreTransform.so
#     ${METADATA_SRC}/libUIDiv.so
#     ${METADATA_SRC}/libUIButton.so
#     ${METADATA_SRC}/libUIMenu.so
#     ${METADATA_SRC}/libTextField.so
#     ${METADATA_SRC}/libAnimator.so
#     ${METADATA_SRC}/libCollider2D.so
#     ${METADATA_SRC}/libAudioSource.so
#     ${METADATA_SRC}/libRigidBody2D.so
#     editorMouseManager
#     usercomponentwrapper
#     entityExplorer
#     cli
#     server
# )



# target_link_libraries(testtarget engine)

# add_library(
#     engine SHARED
#     $(ECS)
#     $(ENGINE)
#     $(SERVER)
#     $(VANILLA)
#     $(META_VANILLA)
# )


# tests
# add_executable(
#     test_generic_storage_system
#     ./src/test_generic_storage_system.cpp
#     )

# target_link_libraries(
#     test_generic_storage_system
#     engine
#     ecs
# )
# add_test(
#     NAME test_generic_storage_system
#     COMMAND test_generic_storage_system
# )
