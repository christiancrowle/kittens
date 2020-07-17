//
// Created by devbat on 6/24/20.
//

#ifndef KITTENS_CHAISCRIPT_CONSOLE_H
#define KITTENS_CHAISCRIPT_CONSOLE_H

#include "../core/SynthBase.h"

namespace Kittens::ChaiScript {
extern int next_console_id;

class ChaiScriptConsole : public Core::SynthBase {
   public:
    ChaiScriptConsole();
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
