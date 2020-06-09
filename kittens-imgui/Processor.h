/*
 * Processor.h
 *
 *  Created on: May 16, 2020
 *      Author: devbat
 */
#include <map>
#include <string>
#include <vector>

#include "DeviceBase.h"
#include "Parameter.h"

#ifndef PROCESSOR_H_
#define PROCESSOR_H_

namespace Kittens::Core {
///
/// \brief The EffectsProcessor interface. Base class to transform samples.
///
class EffectsProcessor : DeviceBase {
   public:
    virtual float process_sample(float s) = 0;  // Take a sample and transform it

    void update_keys(std::array<bool, 512> keys);

    using DeviceBase::get_name;
    using DeviceBase::params;

   protected:
    std::array<bool, 512> keys;
};
}  // namespace Kittens::Core
#endif /* PROCESSOR_H_ */
