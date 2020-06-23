#include <string>
#include <typeinfo>
#include <variant>

#include "aixlog.hpp"
#include "chaiscript/chaiscript.hpp"

#include "../misc/settings.h"
#include "../misc/util.h"
#include "chaiscript_glue.h"
namespace Kittens::ChaiScript {
void update_setting_i(const std::string setting_name, int setting) {
    int i = 0;
    LOG(INFO) << "setting int " << setting_name << "(" << Kittens::Util::string_to_hex(setting_name) << ") to "
              << setting << std::endl;
    Kittens::GlobalSettings[setting_name].set_type("int");
    Kittens::GlobalSettings[setting_name].set_value(setting);
}

void update_setting_s(const std::string setting_name, std::string setting) {
    int i = 0;
    LOG(INFO) << "setting string " << setting_name << "(" << Kittens::Util::string_to_hex(setting_name) << ") to "
              << setting << std::endl;
    Kittens::GlobalSettings[setting_name].set_type("string");
    Kittens::GlobalSettings[setting_name].set_value(setting);
}

void update_setting_f(const std::string setting_name, float setting) {
    int i = 0;
    LOG(INFO) << "setting float " << setting_name << "(" << Kittens::Util::string_to_hex(setting_name) << ") to "
              << setting << std::endl;
    Kittens::GlobalSettings[setting_name].set_type("float");
    Kittens::GlobalSettings[setting_name].set_value(setting);
}

void update_setting_d(const std::string setting_name, double setting) {
    int i = 0;
    LOG(INFO) << "setting double " << setting_name << "(" << Kittens::Util::string_to_hex(setting_name) << ") to "
              << setting << std::endl;
    Kittens::GlobalSettings[setting_name].set_type("double");
    Kittens::GlobalSettings[setting_name].set_value(setting);
}

void update_setting_b(const std::string setting_name, bool setting) {
    int i = 0;
    LOG(INFO) << "setting bool " << setting_name << "(" << Kittens::Util::string_to_hex(setting_name) << ") to "
              << setting << std::endl;
    Kittens::GlobalSettings[setting_name].set_type("bool");
    Kittens::GlobalSettings[setting_name].set_value(setting);
}

void initialize_config(std::string filename) {
    chaiscript::ChaiScript chai;
    chai.add(chaiscript::fun(&update_setting_i), "update_setting");
    chai.add(chaiscript::fun(&update_setting_s), "update_setting");
    chai.add(chaiscript::fun(&update_setting_f), "update_setting");
    chai.add(chaiscript::fun(&update_setting_d), "update_setting");
    chai.add(chaiscript::fun(&update_setting_b), "update_setting");

    chai.eval_file(filename);
}
}  // namespace Kittens::ChaiScript
