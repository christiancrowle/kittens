#include <string>

#include "chaiscript/chaiscript.hpp"

#include "settings.h"

#ifndef CHAISCRIPT_GLUE_H
#define CHAISCRIPT_GLUE_H

namespace Kittens::ChaiScript {
    void initialize_config(std::string config_name);

    static chaiscript::ChaiScript chai;
}
#endif // CHAISCRIPT_GLUE_H
