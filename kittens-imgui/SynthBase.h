/*
 * SynthBase.h
 *
 *  Created on: May 16, 2020
 *      Author: devbat
 */

#include <map>
#include <string>
#include <vector>

#include "Parameter.h"

#ifndef SYNTH_H_
#define SYNTH_H_

namespace Kittens::Core {
///
/// \brief The SynthBase interface. Base class for creating samples.
///
class SynthBase {
   public:
    virtual float get_sample() = 0;

    virtual std::string get_name() = 0;

    void update_keys(std::array<bool, 512> keys);

    std::map<std::string, Parameter> params;

   protected:
    std::string name;
    std::array<bool, 512> keys;
};
}  // namespace Kittens::Core
#endif /* SYNTH_H_ */
