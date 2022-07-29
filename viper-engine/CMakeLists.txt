cmake_minimum_required(VERSION  3.3)

set(CMAKE_CXX_STANDARD 20)

project(ViperEngine)

add_library(ViperEngine SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Viper/demo.cpp
        )

# Set target folders
message(STATUS "Setting up output directories.")
set_target_properties( ViperEngine
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)


# Make sure we also compile glad.c
# message(STATUS "Compiling glad")
# target_sources(Viper PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/glad.c")

# message(STATUS "Compiling GLFW")
# Build settings for GLFW
# set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add GLFW subdirectory and link it
# message(STATUS "Compiling & Linking GLFW")
# add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glfw")
# target_link_libraries(Viper PUBLIC glfw)

# Add GLM subdirectory and link it
# message(STATUS "Compiling & Linking GLM")
# add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glm")
# target_link_libraries(Viper PUBLIC glm)

# Add spldog subdirectory and link it
# message(STATUS "Compiling & Linking SPDLOG")
# add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/spdlog")
# target_link_libraries(Viper PUBLIC spdlog)

# Add nlohmann/json subdirectory and link it
# message(STATUS "Compiling & Linking json")
# add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/json")
# target_link_libraries(Viper PUBLIC json)

# Other includes
target_include_directories(ViperEngine SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

# target_include_directories(Sandbox SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}../src")

# target_link_libraries(Sandbox PUBLIC glfw)
# target_link_libraries(Sandbox PUBLIC glm)
# target_link_libraries(Sandbox PUBLIC spdlog)

# Copy shaders to bin
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/resources/
#        DESTINATION ${CMAKE_BINARY_DIR}/bin/resources/)

# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/resources/imgui.ini
#     DESTINATION ${CMAKE_BINARY_DIR}/bin/)
