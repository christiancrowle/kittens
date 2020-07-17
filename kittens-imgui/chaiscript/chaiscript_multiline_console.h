//
// Created by devbat on 6/24/20.
//

#ifndef KITTENS_CHAISCRIPT_MULTILINE_CONSOLE_H
#define KITTENS_CHAISCRIPT_MULTILINE_CONSOLE_H

#include "../core/SynthBase.h"

namespace Kittens::ChaiScript {
class ChaiScriptMultilineConsole : public Core::SynthBase {
   public:
    ChaiScriptMultilineConsole();
    ChaiScriptMultilineConsole(std::string contents);
    float get_sample();  // returns 0, because a console doesn't make sound
    std::string get_name();

    std::string serialize();

    void queue();
    void stop();

    void render();

    void set_id(int id);
    int get_id();

   protected:
    std::string command_buf;
    int id;
};
}  // namespace Kittens::ChaiScript

#endif  // KITTENS_CHAISCRIPT_CONSOLE_H
