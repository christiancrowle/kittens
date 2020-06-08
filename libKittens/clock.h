//
// Created by devbat on 6/6/20.
//

#include <chrono>
#include <vector>

#ifndef KITTENS_CLOCK_H
#define KITTENS_CLOCK_H

namespace Kittens::Core {
class Clock {
   public:
    short bpm = 120;

    void start();
    bool ready();

    void quantize(uint8_t amount);
    void unquantize();

   private:
    std::chrono::high_resolution_clock::time_point clock_last_tick;
    bool should_quantize = false;
    short clock_pulses_since_last_ready = 0;
    uint8_t quantize_amount;
};
}  // namespace Kittens::Core
#endif  // KITTENS_CLOCK_H
