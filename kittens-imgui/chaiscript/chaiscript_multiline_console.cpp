//
// Created by devbat on 6/24/20.
//

#include "chaiscript_multiline_console.h"
#include <imgui/imgui.h>
#include <aixlog.hpp>
#include "chaiscript_console.h"
#include "chaiscript_glue.h"
#include "cppcodec/base64_rfc4648.hpp"
#include "imgui/misc/cpp/imgui_stdlib.h"

namespace Kittens::ChaiScript {
ChaiScriptMultilineConsole::ChaiScriptMultilineConsole() : Core::SynthBase() {
    this->set_id(3);
}

ChaiScriptMultilineConsole::ChaiScriptMultilineConsole(std::string contents) : ChaiScriptMultilineConsole() {
    LOG(INFO) << "test\n";
    std::vector<unsigned char> content = cppcodec::base64_rfc4648::decode(contents);
    LOG(INFO) << "test\n";
    this->command_buf = std::string(content.begin(), content.end());
    LOG(INFO) << this->command_buf << "\n";
}

float ChaiScriptMultilineConsole::get_sample() {
    return 0;
}

std::string ChaiScriptMultilineConsole::get_name() {
    return "chaiscript console";
}

void ChaiScriptMultilineConsole::queue() {
    Kittens::ChaiScript::eval_buffer(command_buf);
}

void ChaiScriptMultilineConsole::stop() {}

void ChaiScriptMultilineConsole::set_id(int id) {
    this->id = id;
}

int ChaiScriptMultilineConsole::get_id() {
    return id;
}

void ChaiScriptMultilineConsole::render() {
    // ImGui::Begin((std::string("chaiscript console (") + std::to_string(this->id) + ")").c_str());

    ImGui::InputTextMultiline("cmd:", &this->command_buf);
    if (ImGui::Button("run")) {
        Kittens::ChaiScript::eval_buffer(this->command_buf);
    }

    // ImGui::End();
}

std::string ChaiScriptMultilineConsole::serialize() {
    return "cml(" + cppcodec::base64_rfc4648::encode(this->command_buf) + ");";
}
}  // namespace Kittens::ChaiScript