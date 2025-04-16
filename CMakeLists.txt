cmake_minimum_required(VERSION 3.30)
project(Tileset)

set(CMAKE_CXX_STANDARD 17)

set(ASSETS_PATH "${CMAKE_SOURCE_DIR}/assets")

add_compile_definitions(ASSETS="${ASSETS_PATH}")

set(ENGINE_HEADERS
        engine/game/Game.h
        engine/gameState/GameState.h
        engine/gameState/SceneType.h
        engine/scene/Scene.h
        engine/animation/Animation.h
        engine/animation/Animator.h
        game/TileMap.h
)

add_executable(Tileset main.cpp
        engine/game/Game.cpp
        game/test/TestScene.cpp
        game/test/TestScene.h
        game/test/TestSceneType.cpp
        game/test/TestSceneType.h
        game/GameState.cpp
        engine/animation/Animator.cpp
        engine/animation/Animation.cpp
        game/Player.cpp
        game/Player.h
        game/TileMap.cpp
)

find_library(RAYLIB_LIBRARY
        NAMES raylib
        PATHS /opt/homebrew/lib /usr/local/lib
        REQUIRED
)

find_path(RAYLIB_INCLUDE_DIR
        NAMES raylib.h
        PATHS /opt/homebrew/include /usr/local/include
        REQUIRED
)

target_include_directories(Tileset PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/engine
        ${RAYLIB_INCLUDE_DIR}
)

target_link_libraries(Tileset PRIVATE
        ${RAYLIB_LIBRARY}
        "-framework OpenGL"
        "-framework IOKit"
        "-framework Cocoa"
        "-framework CoreVideo"
)

target_compile_features(Tileset PRIVATE cxx_std_17)