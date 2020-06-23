/*
 * SynthBase.h
 *
 *  Created on: May 16, 2020
 *      Author: devbat
 */

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

    void update_keys(std::array<bool, 512> keys);

    using DeviceBase::get_name;
    using DeviceBase::params;
    using DeviceBase::queue;

   protected:
    std::array<bool, 512> keys{};
};
}  // namespace Kittens::Core
#endif /* SYNTH_H_ */
