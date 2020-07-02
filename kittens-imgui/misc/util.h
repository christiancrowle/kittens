//
// Created by devbat on 5/22/20.
//

#include <array>
#include <string>
#include <variant>
#include "SDL2/SDL.h"

#ifndef KITTENS_UTIL_H
#define KITTENS_UTIL_H

namespace Kittens::Util {
std::string file_as_string(std::string filename);

template <class V>
std::type_info const& var_type(V const& v) {
    return std::visit([](auto&& x) -> decltype(auto) { return typeid(x); }, v);
}

template <typename Base, typename T>
inline bool instanceof (const T*) {
    return std::is_base_of<Base, T>::value;
}

std::string string_to_hex(const std::string& input);

constexpr int key_map[] = {SDLK_z, SDLK_x, SDLK_c, SDLK_v, SDLK_b, SDLK_n, SDLK_m, SDLK_a, SDLK_s,
                           SDLK_d, SDLK_f, SDLK_g, SDLK_h, SDLK_j, SDLK_k, SDLK_l, SDLK_q, SDLK_w,
                           SDLK_e, SDLK_r, SDLK_t, SDLK_y, SDLK_u, SDLK_i, SDLK_o, SDLK_p, SDLK_1,
                           SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_0};
}  // namespace Kittens::Util

#endif  // KITTENS_UTIL_H
