#include <string>
#include "../core/SynthBase.h"

#include <SDL2/SDL_scancode.h>
#include <q_io/audio_file.hpp>

namespace q = cycfi::q;

#ifndef SYNTHWAVSAMPLE_H
#define SYNTHWAVSAMPLE_H

namespace Kittens::Instrument {
///
/// \brief The SynthWavSample class. A SynthBase that reads samples from a WAV file.
///
class SynthWavSample : public Core::SynthBase {
   public:
    SynthWavSample(std::string filename);  // filename: the path to load
    SynthWavSample(std::string filename, SDL_Scancode key);
    float get_sample();  // return the next sample from the WAV

    std::string serialize();

    std::string get_name();

    size_t get_sps();  // returns the samples per second of the WAV
    int get_channels();

    void restart();

    void play();
    void stop();

    size_t get_length();

    void queue();

    void render();

   private:
    q::wav_memory wav;
    size_t sps;
    size_t length;
    size_t old_position = 0;
    int channels;
    bool playing = false;
    bool queue_playing = false;
    std::string filename;
};
}  // namespace Kittens::Instrument
#endif  // SYNTHWAVSAMPLE_H
