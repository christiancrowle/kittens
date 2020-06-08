/*
 * Processor.h
 *
 *  Created on: May 16, 2020
 *      Author: devbat
 */
#include <map>
#include <string>
#include <vector>

#include "Parameter.h"

#ifndef PROCESSOR_H_
#define PROCESSOR_H_

namespace Kittens::Core {
///
/// \brief The EffectsProcessor interface. Base class to transform samples.
///
class EffectsProcessor {
   public:
    virtual float process_sample(float s) = 0;  // Take a sample and transform it
    virtual std::string get_name() = 0;

    void update_keys(std::array<bool, 512> keys);

    std::map<std::string, Parameter> params;  // parameters for the processor.
   protected:
    std::string name;
    std::array<bool, 512> keys;
};
}  // namespace Kittens::Core
#endif /* PROCESSOR_H_ */
