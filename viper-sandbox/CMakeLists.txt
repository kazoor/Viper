cmake_minimum_required(VERSION  3.3)

set(CMAKE_CXX_STANDARD 20)

project(ViperSandbox)

add_compile_definitions(VIPER_API VIPER_IMPORT)
get_filename_component(VIPER_ABSOLUTTE_DIR ../ ABSOLUTE)
get_filename_component(VIPER_EDITOR_ABSOLUTTE_DIR ../viper-editor/ ABSOLUTE)


add_executable(ViperSandbox
            ${CMAKE_CURRENT_SOURCE_DIR}/src/sandbox.cpp
        )

message(STATUS "VIPER ABS DIR: " ${VIPER_EDITOR_ABSOLUTTE_DIR} )
target_link_libraries(ViperSandbox PUBLIC ${VIPER_ABSOLUTTE_DIR}/bin/ViperPlayer.dll)

include_directories("${VIPER_EDITOR_ABSOLUTTE_DIR}/external/glfw")
target_link_libraries(ViperSandbox PUBLIC glfw)

# Set target folders
message(STATUS "Setting up output directories.")
set_target_properties( ViperSandbox
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    PREFIX ""
    OUTPUT_NAME "Sandbox"
)
# Other includes
target_include_directories(ViperSandbox SYSTEM PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${VIPER_EDITOR_ABSOLUTTE_DIR}/src/core",
    "${VIPER_EDITOR_ABSOLUTTE_DIR}/src/export",
    "${VIPER_EDITOR_ABSOLUTTE_DIR}/external"
)
include_directories("${VIPER_EDITOR_ABSOLUTTE_DIR}/external/glm/")
include_directories("${VIPER_EDITOR_ABSOLUTTE_DIR}/external/glfw/include/")
include_directories("${VIPER_EDITOR_ABSOLUTTE_DIR}/external/ImGui/")
include_directories("${VIPER_EDITOR_ABSOLUTTE_DIR}/external/entt/single_include/")
include_directories("${VIPER_EDITOR_ABSOLUTTE_DIR}/external/box2d/")