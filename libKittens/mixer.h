#include <vector>
#include <variant>
#include <string>

#include <q_io/audio_stream.hpp>

#include "Parameter.h"
#include "Processor.h"
#include "Synth.h"
#include "chain.h"

namespace q = cycfi::q;

#ifndef MIXER_H
#define MIXER_H

///
/// \brief The Mixer struct. Takes an arbitrary number of Chains and mixes them (weighted according to Chain.volume)
///
struct Mixer : q::port_audio_stream {
public:
    Mixer(size_t sps, int channels);
    void process(out_channels const& out);
    void append_chain(Chain c);
private:
    std::vector<Chain*> chains;
};

#endif // MIXER_H
