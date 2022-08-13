@echo off
glslc.exe viper-vulkan/src/shader.vert -o bin/shader.v.spv
glslc.exe viper-vulkan/src/shader.frag -o bin/shader.f.spv