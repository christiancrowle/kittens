//
// Created by devbat on 6/4/20.
//

#include "SynthBase.h"
#include <algorithm>

namespace Kittens::Core {
void SynthBase::update_keys(std::array<bool, 512> keys) {
    // this->keys = std::array<bool, 512>{};
    this->keys = keys;
}
}  // namespace Kittens::Core