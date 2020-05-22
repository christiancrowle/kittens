#include "synth-wavsample.h"
#include <string>

namespace Kittens::Instrument {
    SynthWavSample::SynthWavSample(std::string filename) : SynthBase(), wav(q::wav_memory{filename}) {
        this->sps = wav.sps();
        this->channels = 2;
        this->length = wav.length();

        this->params["loop_count"] = 2;
    }

    float SynthWavSample::get_sample() {
        return this->wav()[0];
    }

    std::string SynthWavSample::get_name() {
        return "SynthWavSample";
    }

    size_t SynthWavSample::get_sps() {
        return this->sps;
    }

    int SynthWavSample::get_channels() {
        return this->channels;
    }

    size_t SynthWavSample::get_length() {
        return this->length * static_cast<size_t>(std::get<int>(this->params["loop_count"]));
    }
}