cmake_minimum_required(VERSION  3.3)

set(CMAKE_CXX_STANDARD 20)

project(ViperEngine)

add_compile_definitions(VIPER_API VIPER_IMPORT)
get_filename_component(VIPER_ABSOLUTTE_DIR ../ ABSOLUTE)
get_filename_component(VIPER_EDITOR_ABSOLUTTE_DIR ../viper-editor/ ABSOLUTE)


add_executable(ViperEngine
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Viper/editor.cpp
        )

message(STATUS "VIPER ABS DIR: " ${VIPER_EDITOR_ABSOLUTTE_DIR} )
target_link_libraries(ViperEngine PUBLIC ${VIPER_ABSOLUTTE_DIR}/bin/ViperPlayer.dll)

include_directories("${VIPER_EDITOR_ABSOLUTTE_DIR}/external/glfw")
target_link_libraries(ViperEngine PUBLIC glfw)

# Set target folders
message(STATUS "Setting up output directories.")
set_target_properties( ViperEngine
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    PREFIX ""
    OUTPUT_NAME "Editor"
    CXX_STANDARD 20
)
# Other includes
target_include_directories(ViperEngine SYSTEM PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${VIPER_EDITOR_ABSOLUTTE_DIR}/src/core",
    "${VIPER_EDITOR_ABSOLUTTE_DIR}/src/export",
    "${VIPER_EDITOR_ABSOLUTTE_DIR}/external"
)
include_directories("${VIPER_EDITOR_ABSOLUTTE_DIR}/external/glm/")
include_directories("${VIPER_EDITOR_ABSOLUTTE_DIR}/external/glfw/include/")
include_directories("${VIPER_EDITOR_ABSOLUTTE_DIR}/external/ImGui/")
include_directories("${VIPER_EDITOR_ABSOLUTTE_DIR}/external/entt/single_include/")
