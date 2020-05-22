#include <variant>
#include <string>

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

        std::string type;

        std::string name;
    public:
        Parameter* set_name(std::string name);

        Parameter* set_value(std::string newValue);
        Parameter* set_value(int newValue);
        Parameter* set_value(float newValue);
        Parameter* set_value(bool newValue);
        Parameter* set_value(double newValue);

        Parameter* set_type(std::string type);

        std::string get_name();

        std::variant<std::string, int, float, bool, double> get_value();

        Parameter();
    };
}
#endif // PARAMETER_H
