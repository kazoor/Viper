#pragma once
#include <viper/viper.hpp>
namespace Viper::Timestep {
    class VIPER_API Timestep {
    public:
        Timestep( ) : timer( 0.0f ) { };
        Timestep( float time ) : timer( time ) { };
        
        operator float() const { return timer; };
        float seconds() const { return timer; };
        float milliseconds() const { return timer * 1000.0f; };
        float minutes() const { return timer * 1000.0f * 60.0f; };
        float deltatime() const { return seconds(); };
    private:
        float timer;
    };
};