//
// Created by devbat on 5/26/20.
//

#include "imguichain.h"
#include "chain.h"

#include <time.h>
#include <cstdlib>
#include <regex>

#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

namespace Kittens::Imgui {
int random_int() {
    // srand(time(0));
    return rand() % INT32_MAX + 1;
}

void ImguiChain::set_id(long id) {
    this->chain_id = id;
}

long ImguiChain::get_id() {
    return this->chain_id;
}

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

void ImguiChain::MakeNode(int id, std::string name, std::map<std::string, Core::Parameter>* params) {
    imnodes::BeginNode(id);

    imnodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(name.c_str());
    imnodes::EndNodeTitleBar();

    int nextId = -1;
    for (auto& [name, value] : *params) {
        if (value.is_output()) {
            imnodes::BeginOutputAttribute(this->chain_id + (nextId++));
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
            imnodes::BeginInputAttribute(this->chain_id + (nextId++));
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

void ImguiChain::MakeNodeFromProcessor(int id) {
    Core::EffectsProcessor* processor = this->get_processor(id);

    MakeNode(id, node_name(processor, id), &processor->params);
}

void ImguiChain::MakeNodeFromSynth() {
    int id = this->chain_id + this->get_synth_id();
    Core::SynthBase* synth = this->get_synth();

    MakeNode(id, node_name(synth, id), &synth->params);
}

void ImguiChain::Render() {
    next_id = 0;

    this->MakeNodeFromSynth();

    for (auto [id, processor] : this->get_processor_collection()) {
        this->MakeNodeFromProcessor(id);
    }
}
}  // namespace Kittens::Imgui