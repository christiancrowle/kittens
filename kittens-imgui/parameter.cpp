#include "Parameter.h"
#include <algorithm>
#include <string>
#include <variant>
#include "aixlog.hpp"

namespace Kittens::Core {
Parameter::Parameter() {}

Parameter* Parameter::set_type(std::string type) {
    this->type = type;
    return this;
}

Parameter* Parameter::set_name(std::string name) {
    this->name = name;
    return this;
}

Parameter* Parameter::set_value(std::string new_value) {
    this->value_s = new_value;
    return this;
}

Parameter* Parameter::set_value(int new_value) {
    this->value_i = new_value;
    if (is_using_range)
        this->value_i = std::clamp(this->value_i, this->minimum, this->maximum);
    return this;
}

Parameter* Parameter::set_value(float new_value) {
    this->value_f = new_value;
    if (is_using_range)
        this->value_f = std::clamp(this->value_f, static_cast<float>(this->minimum), static_cast<float>(this->maximum));
    return this;
}

Parameter* Parameter::set_value(double new_value) {
    this->value_d = new_value;
    if (is_using_range)
        this->value_d =
            std::clamp(this->value_d, static_cast<double>(this->minimum), static_cast<double>(this->maximum));
    return this;
}

Parameter* Parameter::set_value(bool new_value) {
    this->value_b = new_value;
    return this;
}

std::string Parameter::get_name() {
    return this->name;
}

std::string Parameter::get_type() {
    return this->type;
}

Parameter* Parameter::set_range(int minimum, int maximum) {
    this->maximum = maximum;
    this->minimum = minimum;
    this->is_using_range = true;
    return this;
}

Parameter* Parameter::increase_value(int amount) {
    this->value_i += amount;
    return this;
}

Parameter* Parameter::increase_value(float amount) {
    this->value_f += amount;
    return this;
}

Parameter* Parameter::increase_value(double amount) {
    this->value_d += amount;
    return this;
}

bool Parameter::is_output() {
    return this->output;
}

std::pair<int, int> Parameter::get_range() {
    return std::pair{this->minimum, this->maximum};
}
}  // namespace Kittens::Core