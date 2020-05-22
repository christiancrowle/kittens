#include <string>
#include "settings.h"

#ifndef CHAISCRIPT_GLUE_H
#define CHAISCRIPT_GLUE_H

namespace Kittens::ChaiScript {
    void initialize_config(std::string config_name, Kittens::KittenSettings* settings);
}
#endif // CHAISCRIPT_GLUE_H
