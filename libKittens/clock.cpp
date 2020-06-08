//
// Created by devbat on 6/6/20.
//

#include "clock.h"
#include <chrono>
#include "aixlog.hpp"

#define CLK_PER_BEAT 24
#define SEC_PER_MIN 60

namespace Kittens::Core {
void Clock::start() {
    this->clock_last_tick = std::chrono::high_resolution_clock::now();
}

void Clock::quantize(uint8_t amount) {
    this->should_quantize = true;
    this->quantize_amount = amount;
}

void Clock::unquantize() {
    this->should_quantize = false;
    this->quantize_amount = 0;
}

bool Clock::ready() {
    std::chrono::high_resolution_clock::time_point right_now = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(right_now - clock_last_tick);
    bool is_clock_pulse =
        (ms.count() == (1 / (((bpm * CLK_PER_BEAT) / SEC_PER_MIN) / 1000)));  // see http://archive.is/OdJZ4 for formula

    if (is_clock_pulse)
        this->clock_pulses_since_last_ready++;

    if (this->should_quantize) {
        if (this->clock_pulses_since_last_ready == this->quantize_amount) {
            this->clock_pulses_since_last_ready = 0;
            clock_last_tick = std::chrono::high_resolution_clock::now();
            return true;
        }
    }

    if (is_clock_pulse) {
        this->clock_pulses_since_last_ready = 0;
        clock_last_tick = std::chrono::high_resolution_clock::now();
        return true;
    }

    return false;
}
}  // namespace Kittens::Core