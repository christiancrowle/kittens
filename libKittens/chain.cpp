#include "chain.h"

Chain::Chain() {

}

float Chain::get_sample() {
    float s = this->synth->get_sample();

    for (auto processor : this->processors) { // Process the synth's sample through each processor
        s = processor->process_sample(s);
    }

    return s * this->volume;
}
