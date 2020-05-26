//
// Created by devbat on 5/26/20.
//

#include "chain.h"
#include "imgui/imnodes.h"
#include "Processor.h"
#include "SynthBase.h"

#ifndef KITTENS_IMGUICHAIN_H
#define KITTENS_IMGUICHAIN_H

namespace Kittens::Imgui {

class ImguiChain : public Kittens::Core::Chain {
private:
    int MakeNodeFromProcessor(Kittens::Core::EffectsProcessor *processor);
    int MakeNodeFromSynth(Kittens::Core::SynthBase *synth);

    short next_id = 0;
public:
    void Render();
};

}

#endif //KITTENS_IMGUICHAIN_H
