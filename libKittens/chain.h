#include <utility>
#include <map>

#include "SynthBase.h"
#include "Processor.h"

#ifndef CHAIN_H
#define CHAIN_H

namespace Kittens::Core {
///
/// \brief Chain. Contains exactly one synth and an arbitrary (possibly 0) number of EffectsProcessors.
///
    class Chain {
    public:
        Chain();

        int set_synth(SynthBase* synth);
        int add_processor(EffectsProcessor* processor);

        std::map<int, EffectsProcessor *> get_processor_collection();
        EffectsProcessor* get_processor(int id);
        SynthBase* get_synth();
        int get_synth_id();

        float get_sample(); // get the next sample processed through the chain.
        float volume = 0.7f; // volume. FIXME: expose to script

        short nextId = 0;

    private:
        std::pair<int, SynthBase *> synth; // the synth (plus id)
        std::map<int, EffectsProcessor *> processors; // processors
    };
}
#endif // CHAIN_H
