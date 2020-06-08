#include <map>
#include <string>
#include "Parameter.h"
#include "clock.h"

#ifndef SETTINGS_H
#define SETTINGS_H

namespace Kittens {
extern std::map<std::string, Kittens::Core::Parameter> GlobalSettings;
}  // namespace Kittens

#endif  // SETTINGS_H