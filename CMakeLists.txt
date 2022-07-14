cmake_minimum_required(VERSION  3.3)

set(CMAKE_CXX_STANDARD 17)

project(Viper)

add_executable(Viper
            ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui_demo.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui_draw.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui_impl_glfw.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui_impl_opengl3.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui_widgets.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui_tables.cpp
            )

message(STATUS "Setting up output directories.")
# Set target folders
set_target_properties( Viper
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

message(STATUS "Compiling glad")
# Make sure we also compile glad.c
target_sources(Viper PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/glad.c")

message(STATUS "Compiling GLFW")
# Build settings for GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

message(STATUS "Linking GLFW")
# Add GLFW subdirectory and link it
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glfw")
target_link_libraries(Viper PUBLIC glfw)

# Other includes
target_include_directories(Viper SYSTEM PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/external")