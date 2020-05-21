#include <variant>
#include <string>

#ifndef PARAMETER_H
#define PARAMETER_H

///
/// \brief The Parameter class. DE
///
class Parameter {
private:
    std::variant<std::string, int, float, bool, double> value;
    std::string name;
public:
    void set_name(std::string name);
    void set_value(std::variant<std::string, int, float, bool, double> newValue);
    std::string get_name();
    std::variant<std::string, int, float, bool, double> get_value();
};

#endif // PARAMETER_H
