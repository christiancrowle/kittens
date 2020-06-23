#include <regex>
#include <vector>

#include <imgui/imnodes.h>
#include <time.h>
#include <cstdlib>
#include <regex>

#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "mixer.h"

namespace Kittens::Core {
Mixer::Mixer(size_t sps, int channels) : port_audio_stream(0, channels, sps) {}

std::string node_name(std::variant<Kittens::Core::EffectsProcessor*, Kittens::Core::SynthBase*> n, int id) {
    if (std::holds_alternative<Kittens::Core::EffectsProcessor*>(n))
        return std::get<Kittens::Core::EffectsProcessor*>(n)->get_name() + " (" + std::to_string(id) + ")";
    else
        return std::get<Kittens::Core::SynthBase*>(n)->get_name() + " (" + std::to_string(id) + ")";
}

std::string param_name(std::string name, int id) {
    std::string out = name;
    out = std::regex_replace(out, std::regex("_"), " ");
    return out;
}

void Mixer::MakeNode(int id, std::string name, std::map<std::string, Core::Parameter>* params) {
    imnodes::BeginNode(id);

    imnodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(name.c_str());
    imnodes::EndNodeTitleBar();

    int nextId = -1;
    for (auto& [name, value] : *params) {
        if (value.is_output()) {
            imnodes::BeginOutputAttribute((nextId++));
            ImGui::Text(param_name(name, id).c_str());

            if (value.get_type() == "float") {
                ImGui::SliderFloat(param_name(name, id).c_str(), value.get_value_ref<float>(), value.get_range().first,
                                   value.get_range().second);
            } else if (value.get_type() == "int") {
                ImGui::SliderInt(param_name(name, id).c_str(), value.get_value_ref<int>(), value.get_range().first,
                                 value.get_range().second);
            } else if (value.get_type() == "string") {
                ImGui::InputText(param_name(name, id).c_str(), value.get_value_ref<std::string>());
            } else if (value.get_type() == "bool") {
                ImGui::Checkbox(param_name(name, id).c_str(), value.get_value_ref<bool>());
            }

            imnodes::EndOutputAttribute();
        } else {
            imnodes::BeginInputAttribute((nextId++));
            ImGui::Text(param_name(name, id).c_str());

            if (value.get_type() == "float") {
                ImGui::SliderFloat(param_name(name, id).c_str(), value.get_value_ref<float>(), value.get_range().first,
                                   value.get_range().second);
            } else if (value.get_type() == "int") {
                ImGui::SliderInt(param_name(name, id).c_str(), value.get_value_ref<int>(), value.get_range().first,
                                 value.get_range().second);
            } else if (value.get_type() == "string") {
                ImGui::InputText(param_name(name, id).c_str(), value.get_value_ref<std::string>()->data(),
                                 value.get_range().second);
            } else if (value.get_type() == "bool") {
                ImGui::Checkbox(param_name(name, id).c_str(), value.get_value_ref<bool>());
            }

            imnodes::EndInputAttribute();
        }
    }

    imnodes::EndNode();
}

void Mixer::MakeNodeFromSynth(Core::SynthBase* synth) {
    int id = (next_synth_id++);

    MakeNode(id, node_name(synth, id), &synth->params);
}

void Mixer::Render() {
    next_id = 0;
    next_synth_id = 0;

    for (auto synth : synths) {
        this->MakeNodeFromSynth(synth);
    }
}

void Mixer::process(out_channels const& out) {
    auto left = out[0];
    auto right = out[1];

    for (auto frame : out.frames()) {
        std::vector<float> synth_results;
        float result = 0;

        for (auto synth : this->synths) {
            synth_results.push_back(synth->get_sample() * synth->params["volume"].get_value<float>());
        }

        for (auto sample : synth_results) {
            result = result + sample;
        }

        left[frame] = result;
        right[frame] = result;
    }
}
}  // namespace Kittens::Core