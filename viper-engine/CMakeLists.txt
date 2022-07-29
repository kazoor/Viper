cmake_minimum_required(VERSION  3.3)

set(CMAKE_CXX_STANDARD 20)

project(ViperEngine)

add_library(ViperEngine SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Viper/window/demo.cpp
        )

# Set target folders
message(STATUS "Setting up output directories.")
set_target_properties( ViperEngine
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Other includes
target_include_directories(ViperEngine SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")