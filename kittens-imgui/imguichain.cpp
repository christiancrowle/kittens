//
// Created by devbat on 5/26/20.
//

#include "imguichain.h"
#include "chain.h"

#include <cstdlib>
#include <time.h>
#include <regex>

#include "imgui/imgui.h"

namespace Kittens::Imgui {
    int random_int() {
        //srand(time(0));
        return rand() % INT32_MAX + 1;
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

    void MakeNode(int id, std::string name, std::map<std::string, Core::Parameter> *params) {
        imnodes::BeginNode(id);

        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(name.c_str());
        imnodes::EndNodeTitleBar();

        int nextId = -1;
        for (auto & [name, value] : *params) {
            if (value.is_output()) {
                imnodes::BeginOutputAttribute(nextId++);
                ImGui::Text(param_name(name, id).c_str());

                if (value.get_type() == "float") {
                    ImGui::SliderFloat(param_name(name, id).c_str(), value.get_value_ref<float>(),
                                       value.get_range().first, value.get_range().second);
                } else if (value.get_type() == "int") {
                    ImGui::SliderInt(param_name(name, id).c_str(), value.get_value_ref<int>(),
                                     value.get_range().first, value.get_range().second);
                } else if (value.get_type() == "string") {
                    ImGui::InputText(param_name(name, id).c_str(), value.get_value_ref<std::string>()->data(),
                                     value.get_range().second);
                } else if (value.get_type() == "bool") {
                    ImGui::Checkbox(param_name(name, id).c_str(), value.get_value_ref<bool>());
                }

                imnodes::EndOutputAttribute();
            } else {
                imnodes::BeginInputAttribute(nextId++);
                ImGui::Text(param_name(name, id).c_str());

                if (value.get_type() == "float") {
                    ImGui::SliderFloat(param_name(name, id).c_str(), value.get_value_ref<float>(),
                                       value.get_range().first, value.get_range().second);
                } else if (value.get_type() == "int") {
                    ImGui::SliderInt(param_name(name, id).c_str(), value.get_value_ref<int>(),
                                     value.get_range().first, value.get_range().second);
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
        int id = this->get_synth_id();
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
}