#include "settings.h"

namespace Kittens {
    Parameter KittenSettings::operator[](std::string key) {
           return this->settings[key];
    }
}
