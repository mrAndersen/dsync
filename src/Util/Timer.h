#ifndef DSYNC_TIMER_H
#define DSYNC_TIMER_H

#include <chrono>

class Timer {
public:
    void start();

    void stop();

    bool isStarted();

    void restart();

    float elapsedMilliseconds();

    float elapsedSeconds();

private:
    std::chrono::time_point<std::chrono::system_clock> m_StartTime;
    std::chrono::time_point<std::chrono::system_clock> m_EndTime;
    bool m_bRunning = false;
};

#endif //DSYNC_TIMER_H
