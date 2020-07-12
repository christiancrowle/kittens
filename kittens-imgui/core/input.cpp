//
// Created by devbat on 2020-07-09.
//

#include "input.h"
#include <imgui/imgui.h>
#include <portable-file-dialogs.h>
#include <aixlog.hpp>
#include "../chaiscript/chaiscript_console.h"
#include "../chaiscript/chaiscript_glue.h"
#include "../misc/globalstate.h"

namespace Kittens::Core::Input {

std::map<int, std::vector<std::function<void()>>> SerialBindings = {};
std::vector<char> last_data = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
std::vector<char> data_vec;

void process_data_vec() {
    for (int i = 0; i < data_vec.size(); i++) {
        if (data_vec[i] != last_data[i]) {
            // LOG(INFO) << data_vec[i];
            for (auto bind : SerialBindings[i])
                bind();
        }
    }
    // LOG(INFO) << "\n";

    last_data = data_vec;
    data_vec.clear();
};

void received(const char* data, unsigned int len) {
    std::vector<char> v(data, data + len);

    for (char& c : v) {
        data_vec.push_back(c);
        if (c == '\n')
            process_data_vec();
    }
}

InputHandler::InputHandler(std::string serial_port, int baud) : serial(serial_port, baud) {
    serial.setCallback(received);
}

InputHandler::InputHandler() : serial("", 0) {
    LOG(WARNING) << "serial port disabled\n";
}

void InputHandler::HandleKeyboard(SDL_Event e, ImGuiIO& io) {
    SDL_Scancode key = e.key.keysym.scancode;

    ///
    /// set up imgui io
    ///
    IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
    io.KeysDown[e.key.keysym.scancode] = (e.type == SDL_KEYDOWN);
    io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
    io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
    io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
    io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);

    ///
    /// parameter control mode enabler -- TODO: maybe i should use a map of lambdas for this to clean it up
    ///
    if (e.key.keysym.sym == SDLK_LCTRL) {
        Kittens::GlobalState::IsParameterControlMode = (e.type == SDL_KEYDOWN);
        if (Kittens::GlobalState::IsParameterControlMode)
            SDL_SetRelativeMouseMode(SDL_TRUE);
        else
            SDL_SetRelativeMouseMode(SDL_FALSE);
    }

    ///
    /// imgui input character handling -- TODO: please improve
    ///
    std::string scancode = SDL_GetScancodeName(e.key.keysym.scancode);

    std::map<SDL_Scancode, char> scancode_to_symbol = {
        {SDL_SCANCODE_1, '!'},           {SDL_SCANCODE_2, '@'},           {SDL_SCANCODE_3, '#'},
        {SDL_SCANCODE_4, '%'},           {SDL_SCANCODE_5, '%'},           {SDL_SCANCODE_6, '^'},
        {SDL_SCANCODE_7, '&'},           {SDL_SCANCODE_8, '*'},           {SDL_SCANCODE_9, '('},
        {SDL_SCANCODE_0, ')'},           {SDL_SCANCODE_MINUS, '_'},       {SDL_SCANCODE_APOSTROPHE, '"'},
        {SDL_SCANCODE_LEFTBRACKET, '{'}, {SDL_SCANCODE_RIGHTBRACKET, '}'}};

    if (scancode == "Space")
        scancode = " ";

    if (!io.KeyShift) {
        std::transform(scancode.begin(), scancode.end(), scancode.begin(),
                       [](unsigned char c) { return std::tolower(c); });
    } else {
        if (scancode_to_symbol.count(key) != 0) {
            scancode = scancode_to_symbol[key];
        }
    }

    if (e.type == SDL_KEYUP && scancode.length() == 1)
        io.AddInputCharacter(scancode[0]);

    ///
    /// add chaiscript console on shift+alt+c
    ///
    if ((key == SDL_SCANCODE_C) && (e.type == SDL_KEYUP) && io.KeyShift && io.KeyAlt) {
        Kittens::GlobalState::mixer.synths.push_back(new Kittens::ChaiScript::ChaiScriptConsole());
    }

    ///
    /// save hotkey
    ///
    if ((key == SDL_SCANCODE_S) && (e.type == SDL_KEYUP) && io.KeyCtrl && io.KeyShift) {
        auto f = pfd::save_file("save session...", "./");
        auto f_out = f.result();

        LOG(INFO) << "saving to: " << f_out << "\n";

        Kittens::ChaiScript::serialize_instruments(f_out);
    }

    ///
    /// load hotkey
    ///
    if ((key == SDL_SCANCODE_O) && (e.type == SDL_KEYUP) && io.KeyCtrl && io.KeyShift) {
        auto f = pfd::open_file("save session...", "./");
        auto f_out = f.result()[0];

        LOG(INFO) << "loading from: " << f_out << "\n";

        Kittens::ChaiScript::eval_file(f_out);
    }

    ///
    /// binds for b() script command
    ///
    if (io.KeyAlt && io.KeyCtrl && (key != 0)) {
        if (Kittens::GlobalState::keybinds.count(key) != 0)
            Kittens::GlobalState::keybinds[key]();
    }
}

void InputHandler::HandleSerial() {
    // serial.writeString("s");
}
}  // namespace Kittens::Core::Input