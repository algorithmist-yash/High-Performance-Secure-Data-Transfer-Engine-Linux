#ifndef METRICS_H
#define METRICS_H

#include <chrono>

class Timer {
public:
    void start();
    double stop(); // milliseconds

private:
    std::chrono::high_resolution_clock::time_point begin;
};

#endif
