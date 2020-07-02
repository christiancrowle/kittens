/*
 * SynthBase.h
 *
 *  Created on: May 16, 2020
 *      Author: devbat
 */

#include <SDL2/SDL_scancode.h>
#include <map>
#include <string>
#include <vector>

#include "DeviceBase.h"
#include "Parameter.h"

#ifndef SYNTH_H_
#define SYNTH_H_

namespace Kittens::Core {
///
/// \brief The SynthBase interface. Base class for creating samples.
///
class SynthBase : DeviceBase {
   public:
    virtual float get_sample() = 0;
    virtual std::string serialize() = 0;

    SDL_Scancode get_key();
    void set_key(SDL_Scancode key);

    void set_enabled(bool enabled);
    bool get_enabled();

    virtual void render() = 0;  // for custom rendering code (such as with chaiscriptconsole)

    using DeviceBase::get_name;
    using DeviceBase::params;
    using DeviceBase::queue;

   protected:
    SDL_Scancode key;
    bool enabled = true;
};
}  // namespace Kittens::Core
#endif /* SYNTH_H_ */
