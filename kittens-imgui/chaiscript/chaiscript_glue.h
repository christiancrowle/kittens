#include <string>

#include "chaiscript/chaiscript.hpp"

#include "../misc/settings.h"

#ifndef CHAISCRIPT_GLUE_H
#define CHAISCRIPT_GLUE_H

namespace Kittens::ChaiScript {
void initialize_config(std::string config_name);

static chaiscript::ChaiScript chai;
}  // namespace Kittens::ChaiScript
#endif  // CHAISCRIPT_GLUE_H
