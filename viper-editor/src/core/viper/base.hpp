
#pragma once
//#include <spdlog/spdlog.h>
#include <cstdint>
#include <cstdarg>
#include <memory>
#include <string>

#define VIPER_LOG(fmt, ...) //spdlog::info(fmt, __VA_ARGS__)

#define VIPER_ERR(fmt) spdlog::error(fmt)

#define VIPER_ENV "dev"

#define VIPER_VERSION "0.0.2"

#define VIPER_TITLE "Viper " VIPER_VERSION "-" VIPER_ENV

#define VIPER_ICON_ERR "\xef\x81\x97" //0xEF 0x81 0x97
#define VIPER_ICON_WARN "\xee\x90\x97"
#define VIPER_ICON_INFO "\xef\x81\x9a"
#define VIPER_ICON_SUCC "\xef\x81\x98"

template<typename ... Args>
std::string VIPER_FORMAT_STRING( const std::string& format, Args ... args ) {
    const auto string_size = std::snprintf(nullptr, 0, format.c_str(), std::forward< Args >( args )... ) + 1;

    std::unique_ptr< char[] > buffer( new char[ string_size ] );

    std::snprintf(buffer.get(), string_size, format.c_str(), std::forward< Args >( args )... );
    
    return std::string( buffer.get(), buffer.get() + string_size - 1 );
};

#ifdef _WIN32
    #ifdef VIPER_ENFORCE_NVIDIA_DEDICATED_GRAPHICS
        extern "C" { __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001; }
    #elif VIPER_ENFORCE_RADEON_DEDICATED_GRAPHICS
        extern "C" { __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1; }
    #endif
#endif