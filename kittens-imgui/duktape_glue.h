#include "duktape/duktape.h"
#include "Parameter.h"

#include <map>

#ifndef DUKTAPE_GLUE_H
#define DUKTAPE_GLUE_H

///
/// \brief Namespace containing settings
///
namespace KittenSettings {
    //
    // BEGIN DUKTAPE EXPOSED FUNCTIONS
    //
    static duk_ret_t _update_setting(duk_context *duk_engine); // add or update a setting

    static duk_ret_t _log(duk_context *duk_engine);
    //
    // END DUKTAPE EXPOSED FUNCTIONS
    //

    static std::map<std::string, std::variant<std::string, int, float, bool, double>> settings; // All settings
}

void initialize_duktape(duk_context* duk_engine);

#endif // DUKTAPE_GLUE_H
