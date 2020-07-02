//
// Created by devbat on 6/4/20.
//

#include "SynthBase.h"
#include <SDL2/SDL_scancode.h>
#include <algorithm>

namespace Kittens::Core {
SDL_Scancode SynthBase::get_key() {
    return SDL_SCANCODE_0;
}

void SynthBase::set_enabled(bool enabled) {
    this->enabled = enabled;
}

bool SynthBase::get_enabled() {
    return this->enabled;
}

void SynthBase::set_key(SDL_Scancode key) {
    this->key = key;
}

}  // namespace Kittens::Core
