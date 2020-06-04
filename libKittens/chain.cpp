#include "chain.h"
#include <algorithm>
#include "SDL.h"

namespace Kittens::Core {
Chain::Chain() {}

int Chain::set_synth(SynthBase* synth) {
    int id = nextId++;

    this->synth = std::make_pair(id, synth);

    return id;
}

int Chain::add_processor(EffectsProcessor* processor) {
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

void Chain::update_keys(const bool* keys) {
    copyarray<bool, 512>(this->keys, keys);
}

float Chain::get_sample() {
    this->synth.second->update_keys(this->keys);
    float s = this->synth.second->get_sample();

    for (auto const& [id, processor] : this->processors) {  // Process the synth's sample through each processor
        processor->update_keys(this->keys);
        s = processor->process_sample(s);
    }

    return s * this->volume;
}
}  // namespace Kittens::Core