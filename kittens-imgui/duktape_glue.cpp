#include "duktape_glue.h"

#include "aixlog.hpp"

///
/// \brief internal: _update_setting, JS: update_setting
/// \param internal: duk_engine, JS: setting_name, setting_value, [number_type]
/// \return (void)
///
duk_ret_t _update_setting(duk_context *duk_engine) {
    const char* setting_name = duk_to_string(duk_engine, 0); // param 1, setting_name

    // check type, so we can add it as a variant.
    duk_bool_t is_string = duk_check_type(duk_engine, 1, DUK_TYPE_STRING);
    duk_bool_t is_number = duk_check_type(duk_engine, 1, DUK_TYPE_NUMBER); // JS actually only has one number type. cast later.
    duk_bool_t is_bool = duk_check_type(duk_engine, 1, DUK_TYPE_BOOLEAN);

    if (is_string) {
        std::string s(duk_to_string(duk_engine, 1));

        KittenSettings::settings[setting_name] = s;
    } else if (is_number) {
        std::string type_of_number(duk_to_string(duk_engine, 2)); // get the optional 3rd param (number_type)
        if (type_of_number == "int") {
            int i = (int) duk_get_number(duk_engine, 1); // no i'm NOT going to fucking use a new cast for this shit

            KittenSettings::settings[setting_name] = i;
        } else if (type_of_number == "float") {
            float f = (float) duk_get_number(duk_engine, 1);

            KittenSettings::settings[setting_name] = f;
        } else if (type_of_number == "double") {
            double d = (double) duk_get_number(duk_engine, 1);

            KittenSettings::settings[setting_name] = d;
        }
    } else if (is_bool) { // luckily bools are easy lol
        bool b = (bool) duk_get_boolean(duk_engine, 1);

        KittenSettings::settings[setting_name] = b;
    }

    return 0;
}

///
/// \brief internal: _log, JS: log
/// \param internal: duk_engine, JS: message
/// \return (void)
///
duk_ret_t _log(duk_context *duk_engine) {
    LOG(INFO) << duk_get_string(duk_engine, 0) << std::endl;
    return 0;
}

void initialize_duktape(duk_context* duk_engine) {
    duk_engine = duk_create_heap_default();

    duk_push_c_function(duk_engine, _update_setting, DUK_VARARGS); // see _update_setting above
    duk_put_global_string(duk_engine, "update_setting");
    duk_push_c_function(duk_engine, _log, DUK_VARARGS); // see _log above
    duk_put_global_string(duk_engine, "log");
}
