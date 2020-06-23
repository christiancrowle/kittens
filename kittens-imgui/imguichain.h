//
// Created by devbat on 5/26/20.
//

#include "Processor.h"
#include "SynthBase.h"
#include "chain.h"
#include "imgui/imnodes.h"

#ifndef KITTENS_IMGUICHAIN_H
#define KITTENS_IMGUICHAIN_H

namespace Kittens::Imgui {

class ImguiChain : public Kittens::Core::Chain {
   private:
    void MakeNodeFromProcessor(int id);
    void MakeNodeFromSynth();
    void MakeNode(int id, std::string name, std::map<std::string, Core::Parameter>* params);

    short next_id = 0;
    long chain_id = 0;

   public:
    void Render();
    void set_id(long id);
    long get_id();
};

}  // namespace Kittens::Imgui

#endif  // KITTENS_IMGUICHAIN_H
