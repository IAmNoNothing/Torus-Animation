#pragma once

#include <chrono>
#include <thread>

struct Clock {
    using ClockType = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<ClockType>;
        
    float deltaTime; 
    int currentFPS;  
    TimePoint lastTime;
    unsigned long long ticks = 0;

    Clock();

    void tick(int fps = -1);
};
