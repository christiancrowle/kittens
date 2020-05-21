#include <string>
#include "duktape/duktape.h"

#ifndef CONFIGFILE_H
#define CONFIGFILE_H

void execute_config(std::string configfile, duk_context* duk_engine);

#endif // CONFIGFILE_H
