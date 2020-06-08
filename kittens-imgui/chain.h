#include <algorithm>
#include <map>
#include <utility>

#include "Processor.h"
#include "SynthBase.h"

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

    std::map<int, EffectsProcessor*> get_processor_collection();
    EffectsProcessor* get_processor(int id);
    SynthBase* get_synth();
    int get_synth_id();

    void update_keys(const bool* keys);

    float get_sample();   // get the next sample processed through the chain.
    float volume = 0.7f;  // volume. FIXME: expose to script

    short nextId = 0;

   private:
    std::pair<int, SynthBase*> synth;             // the synth (plus id)
    std::map<int, EffectsProcessor*> processors;  // processors
    std::array<bool, 512> keys;

    template <typename T, size_t N>
    void copyarray(std::array<T, N>& target, const T(*source)) {
        std::copy_n(source, N, std::begin(target));
    }
};
}  // namespace Kittens::Core
#endif  // CHAIN_H
