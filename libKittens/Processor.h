/*
 * Processor.h
 *
 *  Created on: May 16, 2020
 *      Author: devbat
 */
#include <vector>
#include <map>
#include <string>

#include "Parameter.h"

#ifndef PROCESSOR_H_
#define PROCESSOR_H_

namespace Kittens::Core {
///
/// \brief The EffectsProcessor interface. Base class to transform samples.
///
    class EffectsProcessor {
    public:
        virtual float process_sample(float s) = 0; // Take a sample and transform it
        virtual std::string get_name() = 0;

        virtual ~EffectsProcessor() = 0;

        std::map<std::string, Parameter> params; // parameters for the processor.
    protected:
        std::string name;
    };
}
#endif /* PROCESSOR_H_ */
