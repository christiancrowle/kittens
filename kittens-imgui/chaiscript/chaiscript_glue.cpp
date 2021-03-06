#include <string>
#include <typeinfo>
#include <variant>

#include "aixlog.hpp"
#include "chaiscript/chaiscript.hpp"

#include "../core/input.h"
#include "../instrument/synth-wavsample.h"
#include "../misc/globalstate.h"
#include "../misc/settings.h"
#include "../misc/util.h"
#include "chaiscript_console.h"
#include "chaiscript_glue.h"
#include "chaiscript_multiline_console.h"
namespace Kittens::ChaiScript {

chaiscript::ChaiScript* get_chai() {
    static chaiscript::ChaiScript chai;
    return &chai;
}
void update_setting_i(const std::string setting_name, int setting) {
    int i = 0;
    LOG(INFO) << "setting int " << setting_name << "(" << Kittens::Util::string_to_hex(setting_name) << ") to "
              << setting << std::endl;
    Kittens::GlobalSettings[setting_name].set_type("int");
    Kittens::GlobalSettings[setting_name].set_value(setting);
}

void update_setting_s(const std::string setting_name, std::string setting) {
    int i = 0;
    LOG(INFO) << "setting string " << setting_name << "(" << Kittens::Util::string_to_hex(setting_name) << ") to "
              << setting << std::endl;
    Kittens::GlobalSettings[setting_name].set_type("string");
    Kittens::GlobalSettings[setting_name].set_value(setting);
}

void update_setting_f(const std::string setting_name, float setting) {
    int i = 0;
    LOG(INFO) << "setting float " << setting_name << "(" << Kittens::Util::string_to_hex(setting_name) << ") to "
              << setting << std::endl;
    Kittens::GlobalSettings[setting_name].set_type("float");
    Kittens::GlobalSettings[setting_name].set_value(setting);
}

void update_setting_d(const std::string setting_name, double setting) {
    int i = 0;
    LOG(INFO) << "setting double " << setting_name << "(" << Kittens::Util::string_to_hex(setting_name) << ") to "
              << setting << std::endl;
    Kittens::GlobalSettings[setting_name].set_type("double");
    Kittens::GlobalSettings[setting_name].set_value(setting);
}

void update_setting_b(const std::string setting_name, bool setting) {
    int i = 0;
    LOG(INFO) << "setting bool " << setting_name << "(" << Kittens::Util::string_to_hex(setting_name) << ") to "
              << setting << std::endl;
    Kittens::GlobalSettings[setting_name].set_type("bool");
    Kittens::GlobalSettings[setting_name].set_value(setting);
}

void debug_add_synth() {
    Kittens::GlobalState::mixer.synths.push_back(
        new Kittens::Instrument::SynthWavSample("audio_files/Low E.wav", SDL_SCANCODE_D));
    Kittens::GlobalState::mixer.synths.back()->queue();
}

void add_sample(int key, std::string filename) {
    // int newSize = Kittens::GlobalState::mixer.synths.size() + 1;
    // Kittens::GlobalState::mixer.synths.resize(newSize);
    Kittens::GlobalState::mixer.synths.emplace_back(
        new Kittens::Instrument::SynthWavSample(filename, static_cast<SDL_Scancode>(key)));
    // Kittens::GlobalState::mixer.synths.back()->queue();
}

void add_console() {
    Kittens::GlobalState::mixer.synths.push_back(new Kittens::ChaiScript::ChaiScriptConsole());
}

void add_multiline_console() {
    Kittens::GlobalState::mixer.synths.push_back(new Kittens::ChaiScript::ChaiScriptMultilineConsole());
}

void add_multiline_console_with_contents(std::string contents) {
    LOG(INFO) << "test\n";
    Kittens::GlobalState::mixer.synths.push_back(new Kittens::ChaiScript::ChaiScriptMultilineConsole(contents));
}

void queue(int id) {
    Kittens::GlobalState::mixer.synths[id]->queue();
}

void enabled(int id) {
    Kittens::GlobalState::mixer.synths[id]->set_enabled(!Kittens::GlobalState::mixer.synths[id]->get_enabled());
}

void pauseall() {
    for (auto& synth : Kittens::GlobalState::mixer.synths)
        synth->set_enabled(!synth->get_enabled());
}

void queueall() {
    for (auto& synth : Kittens::GlobalState::mixer.synths)
        synth->queue();
}

void bind(int key, const std::string func) {
    Kittens::Core::Input::keybinds[key].push_back(func);
}

void bind_serial(int bit, const std::string func) {
    Kittens::Core::Input::SerialBindings[bit].push_back(func);
}

void save(std::string outfile) {
    serialize_instruments(outfile);
}

void load_file(std::string filename) {
    get_chai()->eval_file(filename);
}

void clear_mixer() {
    Kittens::GlobalState::mixer.Clear();
}

void initialize_config(std::string filename) {
    get_chai()->add(chaiscript::fun(&update_setting_i), "update_setting");
    get_chai()->add(chaiscript::fun(&update_setting_s), "update_setting");
    get_chai()->add(chaiscript::fun(&update_setting_f), "update_setting");
    get_chai()->add(chaiscript::fun(&update_setting_d), "update_setting");
    get_chai()->add(chaiscript::fun(&update_setting_b), "update_setting");
    get_chai()->add(chaiscript::fun(&debug_add_synth), "debug_add_synth");
    get_chai()->add(chaiscript::fun(&add_sample), "s");
    get_chai()->add(chaiscript::fun(&add_console), "c");
    get_chai()->add(chaiscript::fun(&add_multiline_console), "cml");
    get_chai()->add(chaiscript::fun(&add_multiline_console_with_contents), "cmlc");
    get_chai()->add(chaiscript::fun(&queue), "q");
    get_chai()->add(chaiscript::fun(&pauseall), "pause");
    get_chai()->add(chaiscript::fun(&queueall), "queue");
    get_chai()->add(chaiscript::fun(&enabled), "p");
    get_chai()->add(chaiscript::fun(&bind), "b");
    get_chai()->add(chaiscript::fun(&bind_serial), "bs");
    get_chai()->add(chaiscript::fun(&save), "sav");
    get_chai()->add(chaiscript::fun(&load_file), "load");
    get_chai()->add(chaiscript::fun(&clear_mixer), "clr");

    get_chai()->eval_file(filename);
}

void eval_buffer(std::string buf) {
    try {
        get_chai()->eval(buf);
    } catch (std::exception& e) {
        LOG(ERROR) << e.what() << "\n";
    }
}

void eval_file(std::string file) {
    std::ifstream in_fstream(file);
    std::string buffer((std::istreambuf_iterator<char>(in_fstream)), std::istreambuf_iterator<char>());

    in_fstream.close();

    // LOG(INFO) << buffer << "\n";

    eval_buffer(buffer);
}

void serialize_instruments(std::string out_file) {
    std::ostringstream serialization_strings;

    serialization_strings << "clr();\n";

    for (auto synth : Kittens::GlobalState::mixer.synths) {
        serialization_strings << synth->serialize() << "\n";
    }

    for (auto [key, keybind] : Kittens::Core::Input::keybinds) {
        for (auto bind : keybind) {
            serialization_strings << "b(" << key << ", \"" << bind << "\");\n";
        }
    }

    for (auto [idx, serialbind] : Kittens::Core::Input::SerialBindings) {
        for (auto bind : serialbind) {
            serialization_strings << "bs(" << idx << ", \"" << bind << "\");\n";
        }
    }

    std::ofstream out_fstream(out_file);
    out_fstream << serialization_strings.str();
    out_fstream.flush();
    out_fstream.close();
}
}  // namespace Kittens::ChaiScript
