//
// Created by devbat on 5/22/20.
//

#include <string>
#include <variant>

#ifndef KITTENS_UTIL_H
#define KITTENS_UTIL_H

namespace Kittens::Util {
    std::string file_as_string(std::string filename);

    template<class V>
    std::type_info const& var_type(V const& v) {
        return std::visit( [](auto&&x)->decltype(auto){ return typeid(x); }, v );
    }

    std::string string_to_hex(const std::string& input);
}

#endif //KITTENS_UTIL_H
