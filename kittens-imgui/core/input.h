//
// Created by devbat on 2020-07-09.
//

#ifndef KITTENS_INPUT_H
#define KITTENS_INPUT_H

#include <AsyncSerial.h>
#include <SDL_events.h>
#include <imgui/imgui.h>
#include "../misc/settings.h"

namespace Kittens::Core::Input {
extern std::map<int, std::vector<std::function<void()>>> SerialBindings;

class InputHandler {
   private:
    CallbackAsyncSerial serial;

   public:
    void HandleKeyboard(SDL_Event e, ImGuiIO& io);
    void HandleSerial();
    InputHandler(std::string serial_port, int baud);
    InputHandler();
};
}  // namespace Kittens::Core::Input
#endif  // KITTENS_INPUT_H
