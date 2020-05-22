#include <map>
#include <string>
#include "Parameter.h"

#ifndef SETTINGS_H
#define SETTINGS_H

namespace Kittens {
    extern std::map<std::string, Kittens::Core::Parameter> GlobalSettings;
}

#endif //SETTINGS_H