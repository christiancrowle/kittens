#include "Parameter.h"
#include <variant>
#include <string>
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
        return this;
    }

    Parameter* Parameter::set_value(float new_value) {
        this->value_f = new_value;
        return this;
    }

    Parameter* Parameter::set_value(double new_value) {
        this->value_d = new_value;
        return this;
    }

    Parameter* Parameter::set_value(bool new_value) {
        this->value_b = new_value;
        return this;
    }

    std::string Parameter::get_name() {
        return this->name;
    }

    std::variant<std::string, int, float, bool, double> Parameter::get_value() {
        int num_values = 0;
        std::variant<std::string, int, float, bool, double> out;

        if (type == "string") {
            out = this->value_s;
        } else if (type == "int") {
            out = this->value_i;
        } else if (type == "float") {
            out = this->value_f;
        } else if (type == "bool") {
            out = this->value_b;
        } else if (type == "double") {
            out = this->value_d;
        } else {
            LOG(ERROR) << "UH OH! TYPE ISN'T VALID!! EXPECT A CRASH!\n";
        }

        return out;
    }
}