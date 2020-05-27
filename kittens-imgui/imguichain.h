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
    void MakeNodeFromProcessor(int id);
    void MakeNodeFromSynth();

    short next_id = 0;
public:
    void Render();
};

}

#endif //KITTENS_IMGUICHAIN_H
