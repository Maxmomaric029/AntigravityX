cmake_minimum_required(VERSION 3.20)
project(AntiGravityX VERSION 2.4.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add ImGui
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui-master/imgui-master")
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Fetch GLFW
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# Use WIN32 to hide the console window on release builds
set(PROJECT_SOURCES
    src/main.cpp
    src/ui/MainWindow.cpp
    src/ui/Theme.cpp
    src/core/Cleaner.cpp
    src/core/ProcessManager.cpp
    src/core/StartupManager.cpp
    src/core/Tweaks.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${IMGUI_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    src
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw opengl32)
