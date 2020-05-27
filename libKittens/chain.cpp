#include "chain.h"

namespace Kittens::Core {
    Chain::Chain() {

    }

    int Chain::set_synth(SynthBase *synth) {
        int id = nextId++;

        this->synth = std::make_pair(id, synth);

        return id;
    }

    int Chain::add_processor(EffectsProcessor *processor) {
        int id = nextId++;

        this->processors[id] = processor;

        return id;
    }

    SynthBase* Chain::get_synth() {
        return this->synth.second;
    }

    int Chain::get_synth_id() {
        return this->synth.first;
    }

    EffectsProcessor* Chain::get_processor(int id) {
        return this->processors[id];
    }

    std::map<int, EffectsProcessor*> Chain::get_processor_collection() {
        return this->processors;
    }

    float Chain::get_sample() {
        float s = this->synth.second->get_sample();

        for (auto const& [id, processor] : this->processors) { // Process the synth's sample through each processor
            s = processor->process_sample(s);
        }

        return s * this->volume;
    }
}