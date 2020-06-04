#include "synth-wavsample.h"
#include <string>

namespace Kittens::Instrument {
    SynthWavSample::SynthWavSample(std::string filename) : SynthBase(), wav(q::wav_memory{filename}) {
        this->sps = wav.sps();
        this->channels = 2;
        this->length = wav.length();

        this->params["loop_count"].set_type("float");
        this->params["loop_count"].set_value(1.0f);
        this->params["loop_count"].set_range(0, 10);

        this->params["position"].set_type("int");
        this->params["position"].set_value(0);
        this->params["position"].set_range(0, wav.length());
    }

    float SynthWavSample::get_sample() {
        if (*this->params["position"].get_value_ref<int>() != this->old_position) {
            this->wav.seek(*this->params["position"].get_value_ref<int>());
        }

        this->params["position"].set_value(static_cast<int>(this->wav.position()));
        this->old_position = *this->params["position"].get_value_ref<int>();
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

    void SynthWavSample::restart() {
        this->wav.restart();
        this->params["position"].set_value(0);
    }

    size_t SynthWavSample::get_length() {
        return this->length * static_cast<size_t>(std::get<int>(this->params["loop_count"].get_value()));
    }
}