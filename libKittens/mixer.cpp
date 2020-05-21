#include <vector>

#include "mixer.h"

Mixer::Mixer(size_t sps, int channels) : port_audio_stream(0, channels, sps) {

}

void Mixer::process(out_channels const& out) {
    auto left = out[0];
    auto right = out[1];

    for (auto frame : out.frames()) {
        std::vector<float> chain_results;
        float result = 0;

        for (auto chain : this->chains) {
            chain_results.push_back(chain->get_sample());
        }

        for (auto sample : chain_results) {
            result = result + sample;
        }

        left[frame] = result;
        right[frame] = result;
    }
}

void Mixer::append_chain(Chain c) {
    this->chains.push_back(&c);
}
