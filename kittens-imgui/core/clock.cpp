//
// Created by devbat on 6/6/20.
//

#include "clock.h"
#include <chrono>
#include "../misc/settings.h"
#include "aixlog.hpp"

#define CLK_PER_BEAT 24
#define SEC_PER_MIN 60

namespace Kittens::Core {
void Clock::start() {
    this->clock_last_tick = std::chrono::high_resolution_clock::now();
}

bool Clock::is_clock_pulse() {
    std::chrono::high_resolution_clock::time_point right_now = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(right_now - clock_last_tick);

    // LOG(INFO) << ((bpm * CLK_PER_BEAT) / SEC_PER_MIN) / 1000) << "\n";
    // LOG(INFO) << (static_cast<float>(1) / ((static_cast<float>((bpm * CLK_PER_BEAT) / SEC_PER_MIN) / 1000))) << "\n";
    if (Kittens::GlobalSettings["verbose_logging"].get_value<bool>())
        LOG(INFO) << ms.count() << "\n";
    // see http://archive.is/OdJZ4 for formula
    bool is_clock_pulse =
        (ms.count() >= (static_cast<float>(1) / ((static_cast<float>((bpm * CLK_PER_BEAT) / SEC_PER_MIN) / 1000))));

    if (is_clock_pulse)
        this->clock_pulses_since_last_ready++;

    return is_clock_pulse;
}

void Clock::tick() {
    if (this->ready()) {
        for (auto functor : clock_queue) {
            functor();
        }
    }
}

long long Clock::get_pulses_since_last_ready() {
    this->tick();
    return this->clock_pulses_since_last_ready;
}

bool Clock::ready() {
    bool is_clock_pulse = this->is_clock_pulse();

    if (is_clock_pulse) {
        if (Kittens::GlobalSettings["should_quantize"].get_value<bool>()) {
            // LOG(INFO) << Kittens::GlobalSettings["quantize_amount"].get_value<int>() << "\n";
            if (this->clock_pulses_since_last_ready == Kittens::GlobalSettings["quantize_amount"].get_value<int>()) {
                this->clock_pulses_since_last_ready = 0;
                clock_last_tick = std::chrono::high_resolution_clock::now();
                // LOG(INFO) << "clock pulse quantized\n";
                return true;
            }
        } else {
            this->clock_pulses_since_last_ready = 0;
            clock_last_tick = std::chrono::high_resolution_clock::now();
            return true;
        }
    }

    return false;
}

}  // namespace Kittens::Core