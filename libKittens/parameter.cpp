#include "Parameter.h"
#include <variant>
#include <string>

void Parameter::set_name(std::string name) {
    this->name = name;
}

void Parameter::set_value(std::variant<std::string, int, float, bool, double> newValue) {
    this->value = newValue;
}

std::string Parameter::get_name() {
    return this->name;
}

std::variant<std::string, int, float, bool, double> Parameter::get_value() {
    return this->value;
}
