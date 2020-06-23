//
// Created by devbat on 6/6/20.
//

#include <chrono>
#include <functional>
#include <vector>

#ifndef KITTENS_CLOCK_H
#define KITTENS_CLOCK_H

namespace Kittens::Core {
class Clock {
   public:
    short bpm = 120;

    void start();
    bool ready();

    long long get_pulses_since_last_ready();

    bool is_clock_pulse();

    void tick();

    std::vector<std::function<void()>> clock_queue;

   private:
    std::chrono::high_resolution_clock::time_point clock_last_tick;
    long long clock_pulses_since_last_ready;
};
}  // namespace Kittens::Core
#endif  // KITTENS_CLOCK_H
