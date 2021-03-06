#include <string>
#include <variant>

#ifndef PARAMETER_H
#define PARAMETER_H

namespace Kittens::Core {
///
/// \brief The Parameter class. DE
///
class Parameter {
   private:
    std::string value_s;
    int value_i;
    float value_f;
    bool value_b;
    double value_d;

    bool is_using_range = false;
    int maximum = 1;
    int minimum = 0;

    std::string type;

    std::string name;

    bool output = false;

   public:
    Parameter* set_name(std::string name);

    Parameter* set_value(std::string newValue);
    Parameter* set_value(int newValue);
    Parameter* set_value(float newValue);
    Parameter* set_value(bool newValue);
    Parameter* set_value(double newValue);

    Parameter* increase_value(int amount);
    Parameter* increase_value(float amount);
    Parameter* increase_value(double amount);

    Parameter* set_type(std::string type);

    Parameter* set_range(int minimum, int maximum);
    std::pair<int, int> get_range();

    std::string get_type();

    std::string get_name();

    bool is_output();

    template <typename T>
    T* get_value_ref() {
        if constexpr (std::is_same<T, std::string>::value)
            return &this->value_s;
        if constexpr (std::is_same<T, int>::value)
            return &this->value_i;
        if constexpr (std::is_same<T, float>::value)
            return &this->value_f;
        if constexpr (std::is_same<T, bool>::value)
            return &this->value_b;
        if constexpr (std::is_same<T, double>::value)
            return &this->value_d;
    }

    template <typename T>
    T get_value() {
        if constexpr (std::is_same<T, std::string>::value)
            return this->value_s;
        if constexpr (std::is_same<T, int>::value)
            return this->value_i;
        if constexpr (std::is_same<T, float>::value)
            return this->value_f;
        if constexpr (std::is_same<T, bool>::value)
            return this->value_b;
        if constexpr (std::is_same<T, double>::value)
            return this->value_d;
    }

    Parameter();
};
}  // namespace Kittens::Core
#endif  // PARAMETER_H
