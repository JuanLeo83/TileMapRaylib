cmake_minimum_required(VERSION 3.30)
project(Tileset)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)

set(ASSETS_PATH "${CMAKE_SOURCE_DIR}/assets")

add_compile_definitions(ASSETS="${ASSETS_PATH}")

set(IMGUI_VERSION 1.91.8)
FetchContent_Declare(
        imgui
        URL https://github.com/ocornut/imgui/archive/refs/tags/v${IMGUI_VERSION}-docking.tar.gz
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
        glfw
        URL https://github.com/glfw/glfw/archive/refs/tags/3.3.8.tar.gz
)
FetchContent_MakeAvailable(glfw)

find_package(OpenGL REQUIRED)

add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

add_library(ImGuiFileDialog STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/ImGuiFileDialog/ImGuiFileDialog.cpp
)

target_include_directories(ImGuiFileDialog PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${imgui_SOURCE_DIR}
)

target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

set(ENGINE_HEADERS
        engine/game/Game.h
        engine/gameState/GameState.h
        engine/gameState/SceneType.h
        engine/scene/Scene.h
        engine/animation/Animation.h
        engine/animation/Animator.h
        engine/tileMap/TileMap.h
        engine/tileMap/TileMapUtils.h
)

add_executable(Tileset main.cpp
        engine/game/Game.cpp
        engine/animation/Animator.cpp
        engine/animation/Animation.cpp
        engine/tileMap/TileMap.cpp
        game/editor/EditorScene.cpp
        game/editor/EditorScene.h
        game/editor/TestSceneType.cpp
        game/editor/TestSceneType.h
        game/GameState.cpp
        game/Player.cpp
        game/Player.h
        game/editor/GuiManager.cpp
        game/editor/GuiManager.h
)

target_sources(Tileset PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/rlImGui/rlImGui.cpp
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
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/rlImGui
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/ImGuiFileDialog
)

target_link_libraries(Tileset PRIVATE
        ${RAYLIB_LIBRARY}
        "-framework OpenGL"
        "-framework IOKit"
        "-framework Cocoa"
        "-framework CoreVideo"
        imgui
        ImGuiFileDialog
)

target_compile_features(Tileset PRIVATE cxx_std_17)