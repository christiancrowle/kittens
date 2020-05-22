#include <map>
#include <string>
#include "Parameter.h"

#ifndef SETTINGS_H
#define SETTINGS_H

namespace Kittens {
    class KittenSettings {
    private:
        std::map<std::string, Parameter> settings;
    public:
        Parameter operator[](std::string key);
    };

    static KittenSettings GlobalSettings;
}

#endif //SETTINGS_H