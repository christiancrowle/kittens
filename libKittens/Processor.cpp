//
// Created by devbat on 6/4/20.
//

#include "Processor.h"
#include <algorithm>

namespace Kittens::Core {
void EffectsProcessor::update_keys(std::array<bool, 512> keys) {
    this->keys = keys;
};
}  // namespace Kittens::Core