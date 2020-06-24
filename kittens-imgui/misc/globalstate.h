//
// Created by devbat on 6/4/20.
//

#ifndef KITTENS_GLOBALSTATE_H
#define KITTENS_GLOBALSTATE_H

#include <functional>
#include "../core/mixer.h"
namespace Kittens::GlobalState {
extern bool IsParameterControlMode;
extern float ParameterChangeX;
extern float ParameterChangeY;
extern Kittens::Core::Mixer mixer;
extern std::map<int, std::function<void()>> keybinds;
}  // namespace Kittens::GlobalState

#endif  // KITTENS_GLOBALSTATE_H
