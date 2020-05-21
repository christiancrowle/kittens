#include "chain.h"
#include "mixer.h"
#include "synth-wavsample.h"

#include <q/support/literals.hpp>
#include <q/fx/delay.hpp>
#include <q_io/audio_stream.hpp>
#include <q_io/audio_file.hpp>

#include <stdio.h>
#include <unistd.h>

#include <iostream>

namespace q = cycfi::q;

std::string get_working_path() {
   char temp[8192];
   return ( getcwd(temp, sizeof(temp)) ? std::string( temp ) : std::string("") );
}

int main() {
    std::cout << get_working_path() << std::endl;

    SynthWavSample s = SynthWavSample("audio_files/Low E.wav");
    Chain c = Chain();

    c.synth = &s;

    SynthWavSample s2 = SynthWavSample("audio_files/Low D.wav");
    Chain c2 = Chain();

    c2.synth = &s2;

    Mixer m = Mixer(s.get_sps(), s.get_channels());
    m.append_chain(c);
    m.append_chain(c2);

    if (m.is_valid()) {
        m.start();
        q::sleep(s.get_length() / s.get_sps());
        m.stop();
    }
}
