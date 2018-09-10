#include "Timer.h"

void Timer::start() {
    m_StartTime = std::chrono::system_clock::now();
    m_bRunning = true;
}

void Timer::stop() {
    m_EndTime = std::chrono::system_clock::now();
    m_bRunning = false;
}

bool Timer::isStarted() {
    return m_bRunning;
}

void Timer::restart() {
    stop();
    start();
}

float Timer::elapsedMilliseconds() {
    std::chrono::time_point<std::chrono::system_clock> endTime;

    if (m_bRunning) {
        endTime = std::chrono::system_clock::now();
    } else {
        endTime = m_EndTime;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
}

float Timer::elapsedSeconds() {
    return elapsedMilliseconds() / 1000.f;
}