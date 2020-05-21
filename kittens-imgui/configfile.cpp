#include <string>
#include <iostream>
#include <fstream>

#include "duktape/duktape.h"

static void push_file_as_string(duk_context *duk_engine, const char *filename) {
    FILE *f;
    size_t len;
    char buf[16384];

    f = fopen(filename, "rb");
    if (f) {
        len = fread((void *) buf, 1, sizeof(buf), f);
        fclose(f);
        duk_push_lstring(duk_engine, (const char *) buf, (duk_size_t) len);
    } else {
        duk_push_undefined(duk_engine);
    }
}

void execute_config(std::string configfile, duk_context* duk_engine) {
    push_file_as_string(duk_engine, configfile.c_str());
    if (duk_peval(duk_engine) != 0) {
        printf("Error running: %s\n", duk_safe_to_string(duk_engine, -1));
    }
    duk_pop(duk_engine);
}
