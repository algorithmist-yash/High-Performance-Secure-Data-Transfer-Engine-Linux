#include "metrics.h"

void Timer::start() {
    begin = std::chrono::high_resolution_clock::now();
}

double Timer::stop() {
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> diff = end - begin;
    return diff.count();
}
