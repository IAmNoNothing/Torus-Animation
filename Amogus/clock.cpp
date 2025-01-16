#include "clock.h"

void Clock::tick(int fps) {
    TimePoint now = ClockType::now();

    std::chrono::duration<float> elapsedTime = now - lastTime;
    deltaTime = elapsedTime.count();

    if (deltaTime > 0.0f) {
        currentFPS = static_cast<int>(1.0f / deltaTime);
    }

    if (fps > 0) {
        float targetDeltaTime = 1.0f / fps;
        if (deltaTime < targetDeltaTime) {
            auto sleepDuration = std::chrono::duration<float>(targetDeltaTime - deltaTime);
            std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>(sleepDuration));
        }
    }

    lastTime = ClockType::now();
    ticks++;
}

Clock::Clock() : deltaTime(0.0f), currentFPS(0), lastTime(ClockType::now()) {}