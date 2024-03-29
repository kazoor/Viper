cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Viper)

add_compile_definitions(VIPER_API VIPER_EXPORT)
#add_compile_definitions(VIPER_API)

add_library(Viper MODULE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/viper/application.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/viper/application_buffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/window/window.cpp

        # Rendering related files
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/shaders/shader/shader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/renderer/api/vertex.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/renderer/rendercommand.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/renderer/renderer2d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/renderer/renderer3d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/renderer/framebuffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/renderer/sprite2d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/renderer/camera/orthographic_camera.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/renderer/camera/orthographic_camera_controller.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/renderer/camera/scene_camera.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/graphics/renderer/camera/editor_camera.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/layers/layerstack/layerstack.cpp
        
        # Scene related files
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/scene/scene.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/scene/sceneentity.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/scene/scenelayer.cpp

        # Scene -> Panels
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/scene/panels/hierarchy.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/scene/panels/inspector.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/scene/panels/viewport.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/scene/panels/filexplorer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/scene/panels/shadereditor.cpp

        # Input handlers
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/util/input/inputhandler/inputhandler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/util/input/mouse/mouseevents.cpp

        # File handling
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/util/filehandler/filehandler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/util/jsonfilehandler/jsonfilehandler.cpp

        # Misc related files
        
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/util/globals/global.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/src/core/util/physics/physics2d.cpp

        # Vendor files
        ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui_impl_glfw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui_impl_opengl3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui_widgets.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/ImGuizmo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/ImSequencer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGui/ImGradient.cpp
        )

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/box2d/include/box2d/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/stb/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/glm/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/entt/single_include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/fast_obj/")

# Set target folders
message(STATUS "Setting up output directories.")
set_target_properties( Viper
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        PREFIX ""
        OUTPUT_NAME "ViperPlayer"
        CXX_STANDARD 20
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

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/entt")

# Add nlohmann/json subdirectory and link it
message(STATUS "Compiling & Linking json")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/json")
target_link_libraries(Viper PUBLIC nlohmann_json)


message(STATUS "Compiling & Linking box2d")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/box2d")
target_link_libraries(Viper PUBLIC box2d)

# Other includes
target_include_directories(Viper SYSTEM PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/external"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/core")

# Copy shaders to bin
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/resources/
        DESTINATION ${CMAKE_BINARY_DIR}/bin/resources/)