@echo off
ECHO Building Viper engine
cmake -S . -DCMAKE_BUILD_TYPE=Release -DSPDLOG_BUILD_EXAMPLES=OFF -DSPDLOG_BUILD_BENCH=OFF -DSPDLOG_BUILD_TESTS=OFF -G "MinGW Makefiles"
cmake --build .
cd bin && start Editor.exe
ECHO Building Viper engine done