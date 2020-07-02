#include <string>

#include "chaiscript/chaiscript.hpp"

#include "../misc/settings.h"

#ifndef CHAISCRIPT_GLUE_H
#define CHAISCRIPT_GLUE_H

namespace Kittens::ChaiScript {
void initialize_config(std::string config_name);

void eval_buffer(std::string buf);

void serialize_instruments(std::string out_file);

extern std::string command_buf;
}  // namespace Kittens::ChaiScript
#endif  // CHAISCRIPT_GLUE_H
