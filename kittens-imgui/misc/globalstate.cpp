//
// Created by devbat on 6/4/20.
//

#include "globalstate.h"

namespace Kittens::GlobalState {
bool IsParameterControlMode = false;
float ParameterChangeX = 0.0f;
float ParameterChangeY = 0.0f;
Kittens::Core::Mixer mixer = Kittens::Core::Mixer(44100, 2);
std::map<int, std::function<void()>> keybinds;
}  // namespace Kittens::GlobalState