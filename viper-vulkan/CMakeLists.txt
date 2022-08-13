cmake_minimum_required(VERSION  3.3)

set(CMAKE_CXX_STANDARD 20)

project(ViperVulkan)

add_executable(ViperVulkan
            ${CMAKE_CURRENT_SOURCE_DIR}/src/instance.cpp
        )

if (DEFINED VULKAN_SDK_PATH)
    set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include") # 1.1 Make sure this include path is correct
    set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib") # 1.2 Make sure lib path is correct
    set(Vulkan_FOUND "True")
else()
    find_package(Vulkan REQUIRED)
    message(STATUS "Found vulkan: $ENV{VULKAN_SDK}")
endif()
if (NOT Vulkan_FOUND)
    message(STATUS "Couldn't find Vulkan D:")
else()
    message(STATUS "Using vulkan lib at ${Vulkan_LIBRARIES}")
endif()

# Set target folders
message(STATUS "Setting up output directories.")
set_target_properties( ViperVulkan
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    PREFIX ""
    OUTPUT_NAME "Vulkan"
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories("${Vulkan_INCLUDE_DIRS}")

message(STATUS "Compiling & Linking GLFW")

include_directories("${VIPER_EDITOR_ABSOLUTTE_DIR}/external/glfw")
target_link_libraries(ViperVulkan PUBLIC glfw)

include_directories("${VIPER_EDITOR_ABSOLUTTE_DIR}/external/spdlog")
target_link_libraries(ViperVulkan PUBLIC spdlog)

target_link_libraries(ViperVulkan PUBLIC ${Vulkan_LIBRARIES})

target_include_directories(ViperVulkan SYSTEM PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor"
        "${Vulkan_INCLUDE_DIRS}" )