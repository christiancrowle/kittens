//
// Created by devbat on 6/24/20.
//

#include "chaiscript_console.h"
#include <imgui/imgui.h>
#include "chaiscript_glue.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

namespace Kittens::ChaiScript {
int next_console_id = 0;

ChaiScriptConsole::ChaiScriptConsole() : Core::SynthBase() {
    this->set_id(Kittens::ChaiScript::next_console_id++);
}

float ChaiScriptConsole::get_sample() {
    return 0;
}

std::string ChaiScriptConsole::get_name() {
    return "chaiscript console";
}

void ChaiScriptConsole::queue() {
    Kittens::ChaiScript::eval_buffer(command_buf);
}

void ChaiScriptConsole::set_id(int id) {
    this->id = id;
}

int ChaiScriptConsole::get_id() {
    return id;
}

void ChaiScriptConsole::render() {
    // ImGui::Begin((std::string("chaiscript console (") + std::to_string(this->id) + ")").c_str());

    ImGui::InputText("cmd:", &this->command_buf);
    ImGui::SameLine();
    if (ImGui::Button("run")) {
        Kittens::ChaiScript::eval_buffer(this->command_buf);
    }

    // ImGui::End();
}
}  // namespace Kittens::ChaiScript