//
// Created by devbat on 5/26/20.
//

#include "imguichain.h"

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

    void MakeNode(int id, std::string name, std::map<std::string, std::variant<std::string, int, float, bool, double>> *params) {
        imnodes::BeginNode(id);

        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(name.c_str());
        imnodes::EndNodeTitleBar();

        int nextId = -1;
        for (auto const& [name, value] : *params) {
            imnodes::BeginInputAttribute(nextId++);
            ImGui::Text(param_name(name, id).c_str());
            imnodes::EndInputAttribute();
        }

        imnodes::EndNode();
    }

    int ImguiChain::MakeNodeFromProcessor(Kittens::Core::EffectsProcessor* processor) {
        int id = next_id++;
        MakeNode(id, node_name(processor, id), &processor->params);
        return id;
    }

    int ImguiChain::MakeNodeFromSynth(Kittens::Core::SynthBase* synth) {
        int id = next_id++;
        MakeNode(id, node_name(synth, id), &synth->params);
        return id;
    }

    void ImguiChain::Render() {
        next_id = 0;

        this->MakeNodeFromSynth(this->synth);

        for (auto processor : this->processors) {
            this->MakeNodeFromProcessor(processor);
        }
    }
}