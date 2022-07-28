cmake_minimum_required(VERSION  3.3)

set(CMAKE_CXX_STANDARD 20)

project(Viper)

add_library(Viper
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/window/application.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/window/window.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/shaders/shader/shader.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/layers/layerstack/layerstack.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/renderer/renderer.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/renderer/vertexbuffer.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/renderer/indexbuffer.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/renderer/framebuffer.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/renderer/vertexarray.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/renderer/sprite2d.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/renderer/camera/orthographic_camera.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/components/component.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/components/transform.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/components/spriterenderer.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/components/gameobject.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/components/camera.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/components/gom.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/components/boxcollision2d.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/components/input.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/util/filehandler/filehandler.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/util/input/inputhandler/inputhandler.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/util/filehandler/filehandler.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/util/input/mouse/mouseevents.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/core/util/globals/global.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui_demo.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui_draw.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui_impl_glfw.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui_impl_opengl3.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui_widgets.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui_tables.cpp
        )

# Set target folders
message(STATUS "Setting up output directories.")
set_target_properties( Viper
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)


# Make sure we also compile glad.c
message(STATUS "Compiling glad")
target_sources(Viper PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/glad.c")

message(STATUS "Compiling GLFW")
# Build settings for GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add GLFW subdirectory and link it
message(STATUS "Compiling & Linking GLFW")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glfw")
target_link_libraries(Viper PUBLIC glfw)

# Add GLM subdirectory and link it
message(STATUS "Compiling & Linking GLM")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glm")
target_link_libraries(Viper PUBLIC glm)

# Add spldog subdirectory and link it
message(STATUS "Compiling & Linking SPDLOG")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/spdlog")
target_link_libraries(Viper PUBLIC spdlog)

# Add nlohmann/json subdirectory and link it
message(STATUS "Compiling & Linking json")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/json")
target_link_libraries(Viper PUBLIC json)

# Other includes
target_include_directories(Viper SYSTEM PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/external")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/stb/")

# Sandbox
set(SANDBOX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sandbox/)
add_executable(Sandbox ${SANDBOX_DIR}/main.cpp)

set_target_properties( Sandbox
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )

# target_include_directories(Sandbox SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}../src")
target_link_libraries(Sandbox PUBLIC ${CMAKE_BINARY_DIR}/lib/libViper.a)

target_link_libraries(Sandbox PUBLIC glfw)
target_link_libraries(Sandbox PUBLIC glm)
target_link_libraries(Sandbox PUBLIC spdlog)

# Copy shaders to bin
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/resources/
        DESTINATION ${CMAKE_BINARY_DIR}/bin/resources/)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/resources/imgui.ini
    DESTINATION ${CMAKE_BINARY_DIR}/bin/)
