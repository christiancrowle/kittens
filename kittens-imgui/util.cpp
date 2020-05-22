//
// Created by devbat on 5/22/20.
//

#include "util.h"
#include <fstream>
#include <iostream>
#include <vector>

namespace Kittens::Util {
    std::string file_as_string(std::string filename) {
        std::ifstream stream(filename, std::ios::binary | std::ios::ate);
        std::streamsize size = stream.tellg();
        stream.seekg(0, std::ios::beg);

        std::string buffer;
        if (stream.read(buffer.data(), size)) {
            return buffer;
        }

        return "";
    }

    std::string string_to_hex(const std::string& input) {
        static const char hex_digits[] = "0123456789ABCDEF";

        std::string output;
        output.reserve(input.length() * 2);
        for (unsigned char c : input)
        {
            output.push_back(hex_digits[c >> 4]);
            output.push_back(hex_digits[c & 15]);
        }
        return output;
    }
}