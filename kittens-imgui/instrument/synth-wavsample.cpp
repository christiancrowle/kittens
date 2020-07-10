#include "synth-wavsample.h"
#include "../core/Status.h"
#include "../misc/globalstate.h"
#include "../misc/settings.h"

#include <imgui/imgui.h>
#include <algorithm>
#include <sstream>
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

    this->params["comment"].set_type("string");
    this->params["comment"].set_value("");
    this->params["comment"].set_range(0, 255);

    this->params["volume"].set_type("float");
    this->params["volume"].set_value(0.5f);
    this->params["volume"].set_range(0, 1);

    this->filename = filename;
}

SynthWavSample::SynthWavSample(std::string filename, SDL_Scancode key) : SynthWavSample(filename) {
    this->set_key(key);
}

void SynthWavSample::queue() {
    if (Kittens::GlobalSettings["clocked_mode"].get_value<bool>()) {
        // oof ouch owie my lambdas
        Kittens::Status::clock.clock_queue.emplace_back(std::bind(&SynthWavSample::play, this));
    } else {
        this->play();
    }
}

void SynthWavSample::play() {
    this->restart();
    this->queue_playing = true;
}

void SynthWavSample::stop() {
    this->playing = false;
}

float SynthWavSample::get_sample() {
    if (this->get_enabled()) {
        if (this->queue_playing) {
            if (Kittens::Status::clock.ready()) {
                this->playing = true;
                this->queue_playing = false;
            }

            if (!Kittens::GlobalSettings["clocked_mode"].get_value<bool>()) {
                this->playing = true;
                this->queue_playing = false;
            }
        }

        if (Kittens::GlobalState::IsParameterControlMode && ImGui::GetIO().KeysDown[this->key]) {
            this->params["volume"].increase_value(Kittens::GlobalState::ParameterChangeY / 1000);
        }

        if (this->playing) {
            if (this->params["position"].get_value<int>() != this->old_position) {
                this->wav.seek(this->params["position"].get_value<int>());
            }

            this->params["position"].set_value(static_cast<int>(this->wav.position()));
            this->old_position = this->params["position"].get_value<int>();

            if (this->wav.position() == this->wav.length()) {
                this->restart();
                this->playing = false;
            }

            return this->wav()[0];
        } else {
            return 0;  // do nothing if not playing.
        }
    } else {
        return 0;
    }
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
    return this->length * static_cast<size_t>(this->params["loop_count"].get_value<int>());
}

void SynthWavSample::render() {}

std::string SynthWavSample::serialize() {
    std::ostringstream serialization_string;
    serialization_string << "s(";
    serialization_string << std::to_string(this->key);
    serialization_string << ", \"" << this->filename << "\");";

    return serialization_string.str();
}
}  // namespace Kittens::Instrument