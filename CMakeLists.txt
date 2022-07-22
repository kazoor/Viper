cmake_minimum_required(VERSION  3.3)

set(CMAKE_CXX_STANDARD 20)

project(Viper)

add_library(Viper
            ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/window/window.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/shaders/shader/shader.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/layers/layerstack/layerstack.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/renderer/renderer.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/util/input/inputhandler/inputhandler.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/util/filehandler/filehandler.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/util/input/mouse/mouseevents.cpp
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

message(STATUS "Compiling & Linking SPDLOG")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/spdlog")
target_link_libraries(Viper PUBLIC spdlog)

# Other includes
target_include_directories(Viper SYSTEM PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/external")

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
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/resources/
        DESTINATION ${CMAKE_BINARY_DIR}/bin/resources/)
