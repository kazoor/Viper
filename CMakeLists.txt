cmake_minimum_required(VERSION  3.3)
project(Viper-all)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/viper-editor)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/viper-engine)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/viper-sandbox)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/viper-editor/src/resources/imgui.ini
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/)