#pragma once
#include <chrono>
namespace Viper::Timer {
    class Timer {
    public:
        Timer() {
            Reset();
        };

        void Reset() {
            m_Time = std::chrono::high_resolution_clock::now();
        };

        float Elapsed() {
            return std::chrono::duration_cast< std::chrono::nanoseconds >( std::chrono::high_resolution_clock::now() - m_Time ).count() * 0.001f * 0.001f * 0.001f;
        };

        float Milliseconds() {
            return Elapsed() * 1000.0f;
        };
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_Time;
    };
};