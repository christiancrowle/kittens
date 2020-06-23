#include <string>
#include <variant>
#include <vector>

#include <q_io/audio_stream.hpp>

#include "Parameter.h"
#include "Processor.h"
#include "SynthBase.h"
#include "chain.h"

namespace q = cycfi::q;

#ifndef MIXER_H
#define MIXER_H

namespace Kittens::Core {
///
/// \brief The Mixer struct. Takes an arbitrary number of Chains and mixes them (weighted according to Chain.volume)
///
struct Mixer : q::port_audio_stream {
   public:
    std::vector<SynthBase*> synths;

    Mixer(size_t sps, int channels);

    void process(out_channels const& out);
    void Render();

   private:
    int next_id;
    int next_synth_id;

    void MakeNode(int id, std::string name, std::map<std::string, Core::Parameter>* params);
    void MakeNodeFromSynth(Core::SynthBase* synth);
};
}  // namespace Kittens::Core
#endif  // MIXER_H
